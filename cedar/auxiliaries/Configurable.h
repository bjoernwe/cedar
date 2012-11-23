/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
    This file is part of cedar.

    cedar is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    cedar is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with cedar. If not, see <http://www.gnu.org/licenses/>.

========================================================================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        Configurable.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 06

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_CONFIGURABLE_H
#define CEDAR_AUX_CONFIGURABLE_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/threadingUtilities.h"
#include "cedar/auxiliaries/Log.h"

// SYSTEM INCLUDES
#include <map>
#include <list>
#include <fstream> // for write_csv()
#include <string>  // for write_csv()
#include <ostream> // for write_csv()
#include <iomanip> // for write_csv()
#include <iostream> // for write_csv() debugging
#include <boost/next_prior.hpp> // for write_csv()
#include <boost/signals2/signal.hpp>
#include <boost/signals2/connection.hpp>
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

/*!@brief An interface for classes that can store and load parameters from files.
 */
class cedar::aux::Configurable : public boost::noncopyable
{
  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------
  friend class cedar::aux::Parameter;

  //--------------------------------------------------------------------------------------------------------------------
  // macros and types
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief list of cedar::aux::ParameterPtr
  typedef std::list<cedar::aux::ParameterPtr> ParameterList;
  //!@brief map of parameter names to its place in ParameterList, expressed as iterator
  typedef std::map<std::string, ParameterList::iterator> ParameterMap;
  //!@brief map of children of a single cedar::aux::Configurable, pointing from their name to their pointers
  typedef std::map<std::string, cedar::aux::ConfigurablePtr> Children;
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  Configurable();

  //!@brief Destructor
  virtual ~Configurable();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief read a configuration for all registered parameters from a cedar::aux::ConfigurationNode
  virtual void readConfiguration(const cedar::aux::ConfigurationNode& node);

  /*!@brief create a tree of type cedar::aux::ConfigurationNode from a json file and try to read the configuration
   * afterward
   */
  void readJson(const std::string& filename);

  /*!@brief   Reads the configuration from an INI file.
   *
   * @remarks This method is used for providing downward compatibility to the old config interface. Please don't use it
   *          for anything else!
   */
  void readOldConfig(const std::string& filename);

  /*!@brief   Writes the configuration to an INI file.
   *
   * @remarks This method is used for providing downward compatibility to the old config interface. Please don't use it
   *          for anything else!
   */
  void writeOldConfig(const std::string& filename);

  //!@brief write a configuration for all registered parameters to a root node of type cedar::aux::ConfigurationNode
  virtual void writeConfiguration(cedar::aux::ConfigurationNode& root) const;

  //!@brief write a configuration to a cedar::aux::ConfigurationNode tree and store this tree in a json file
  void writeJson(const std::string& filename) const;

  //!@brief write a configuration to a cedar::aux::ConfigurationNode tree and store this tree in a csv spreadsheet file
  void writeCsv(const std::string& filename, const char separator = ',') const;

  //!@brief get a map of all children of the current Configurable
  const Children& configurableChildren() const;

  //!@brief get a const list of all parameters registered at this Configurable
  const ParameterList& getParameters() const;

  //!@brief get a list of all parameters registered at this Configurable
  ParameterList& getParameters();

  /*!@brief add a Configurable as a child to this instance of Configurable - if name is duplicate, an exception is
   * thrown
   */
  void addConfigurableChild(const std::string& name, cedar::aux::ConfigurablePtr child);

  //!@brief removes a child configuration from the configuration tree - if name is not found, an exception is thrown
  void removeConfigurableChild(const std::string& name);

  /*!@brief register a function pointer with this function to react to any changes in the tree structure (for example,
   * removing a child node)
   */
  boost::signals2::connection connectToTreeChangedSignal(boost::function<void ()> slot);

  /*!@brief Resets the changed flag of all parameters associated with this Configurable.
   */
  void resetChangedStates(bool newChangedFlagValue) const;

  //!@brief copy a configuration from another instance of the same class (type check included)
  void copyFrom(ConstConfigurablePtr src);

  //!@brief copy a configuration to another instance of the same class (type check included)
  void copyTo(ConfigurablePtr target) const;

  /*!@brief Returns the parameter associated with the path.
   * @todo This should be const, but that conflicts with the intrusive pointers
   */
  cedar::aux::ParameterPtr getParameter(const std::string& path);

  /*!@brief Returns the configurable child associated with the path.
   * @todo This should be const, but that conflicts with the intrusive pointers in getParameter
   */
  cedar::aux::ConfigurablePtr getConfigurableChild(const std::string& path);

  /*!@brief Locks all parameters of the configurable.
   */
  void lockParameters(cedar::aux::LOCK_TYPE lockType) const;

  /*!@brief Unlocks all parameters of the configurable.
   */
  void unlockParameters() const;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief emits a "changed" signal for every parameter that was loaded
  virtual void configurationLoaded();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  /*!@brief register a new parameter at this Configurable - throws an exception if parameter is already part of this
   * Configurable
   */
  void registerParameter(cedar::aux::Parameter* parameter);

  //!@brief Transforms the old config format to one readable in the new interface.
  void oldFormatToNew(cedar::aux::ConfigurationNode& node);

  //!@brief Transforms the old config format to one readable in the new interface.
  void newFormatToOld(cedar::aux::ConfigurationNode& node);

  //!@brief Sets all parameters to their default values.
  void defaultAll();

  //!@brief Updates the lock set of the configurable.
  void updateLockSet();

  //!@brief Appends the locks of this configurable and its children to the set.
  void appendLocks(std::set<QReadWriteLock*>& locks);

  //!@brief make sure the directories exist 
  std::string normalizeFilename(const std::string& filename) const;

  //!@brief helper function to write a Ptree to .csv. internals start here. opens the stream
  template<class Ptree>
  void writeCsvConfiguration(const std::string& filename, const Ptree& pt, 
                             const char separator) const
  {
    // heavily copied + simplified from boost
    std::basic_ofstream<typename Ptree::key_type::value_type>
    stream(filename.c_str());
    if (!stream)
    {
      cedar::aux::LogSingleton::getInstance()->warning
      (
        "cannot write file: " + filename,
        "cedar::aux::Configurable::writeCsv()"
      );
    }
    writeCsvStream(stream, pt, filename, separator);
  }


  //!@brief helper function to write a Ptree to .csv. internal. check stream
  template<class Ptree>
  void writeCsvStream(std::basic_ostream<typename Ptree::key_type::value_type>& stream, 
                            const Ptree& pt,
                            const std::string& filename,
                            const char separator) const
  {

    writeCsvItem(stream, pt, 0, separator);
    stream << std::endl;
    if (!stream.good())
    {
      cedar::aux::LogSingleton::getInstance()->warning
      (
        "write error: " + filename,
        "cedar::aux::Configurable::writeCsv()"
      );

    }
  }

  //!@brief helper function to write a Ptree to .csv. internal. write structure
  template<class Ptree>
  void writeCsvItem(std::basic_ostream<typename Ptree::key_type::value_type>& stream, 
                        const Ptree& pt, int indent,
                        const char separator) const
  {
    typedef typename Ptree::key_type::value_type Ch;
    typedef typename std::basic_string<Ch> Str;

    // vectors of Parameters fully written into one row:
    // one value per cell;
    // every level 0 container element starts new row;
    // all level >= 1 containers with non-values also start new row
    // rows first cell is always the containers Parameter name;
    //     A   B   C   D   E 
    //    name 42  43  44  45

    if (pt.empty() && indent > 0)
    {      
      // write value:
      //<< std::setprecision(4) // set decimal precision for data. saves a lot of bytes ;)
      stream << ( pt.template get_value<Str>() ); 
        // TODO: add escaping special characters
    }
    else if (pt.count(Str()) == pt.size() && indent > 0)
    {
      // sub-array:
      typename Ptree::const_iterator it = pt.begin();
      for (; it != pt.end(); ++it)
      {
        // write-out one level lower:
        writeCsvItem(stream, it->second, indent + 1, separator);
        if (boost::next(it) != pt.end())
        {
          // separator for cells (columns):
          stream << Ch(separator);
        }
      }
    }
    else
    {
      // iterate over first level:
      typename Ptree::const_iterator it = pt.begin();
      for (; it != pt.end(); ++it)
      {
        stream << Ch('\n'); // every entry on new line (row)
        stream << (it->first) << Ch(separator); // name in first cell of row

        writeCsvItem(stream, it->second, indent + 1, separator);
      }

      stream << Ch('\n'); // NL on last row
    }
  }

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief a boost signal that is emitted if a change in the tree structure appears
  boost::signals2::signal<void ()> mTreeChanged;
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  //!@brief list of parameters registered at this Configurable instance
  ParameterList mParameterList;

  //!@brief map of registered parameters, using the parameter name as key
  ParameterMap mParameterAssociations;

  //!@brief map of children of this Configurable instance
  Children mChildren;

  /*!@brief   The lockable used for locking this configurable and all its parameters.
   *
   * @remarks In order to avoid multiple inheritance down the line, configurable has, rather than is, a lockable.
   */
  mutable std::set<QReadWriteLock*> mParameterLocks;
}; // class cedar::aux::Configurable

#endif // CEDAR_AUX_CONFIGURABLE_H
