/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        Settings.h

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2013 06 21

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_SETTINGS_H
#define CEDAR_AUX_SETTINGS_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/BoolParameter.h"
#include "cedar/auxiliaries/SetParameter.h"
#include "cedar/auxiliaries/StringVectorParameter.h"

// SYSTEM INCLUDES

/*!@brief A singleton class for storing user-specific parameters related to the processing framework.
 *
 * @todo  Write a widget for these settings.
 */
class cedar::aux::Settings : public cedar::aux::Configurable
{
  //--------------------------------------------------------------------------------------------------------------------
  // friend
  //--------------------------------------------------------------------------------------------------------------------
  friend class cedar::aux::Singleton<cedar::aux::Settings>;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief The standard constructor.
  Settings();

public:
  //!@brief The destructor.
  ~Settings();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Loads the settings from a file in the user's home directory.
   */
  void load();

  /*!@brief Saves the settings to a file in the user's home directory.
   */
  void save();

  //! Whether or not memory output is generated.
  bool getMemoryDebugOutput();

  //!@brief returns a list of all plugins that should be loaded on start-up
  const std::set<std::string>& pluginsToLoad();

  //! Returns the plugins search paths.
  const std::vector<std::string>& getSearchPaths() const;

  //!@brief adds a plugin to the list of plugins that are loaded on start-up
  void addPluginToLoad(const std::string& path);

  //!@brief removes a plugin from the list of plugins that are loaded on start-up
  void removePluginToLoad(const std::string& path);

  //! Loads the plugins set to be loaded by default.
  void loadDefaultPlugins();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief Parameter representing the plugin workspace.
  cedar::aux::BoolParameterPtr _mMemoryDebugOutput;

  //!@brief List of plugins that should be loaded on startup.
  cedar::aux::StringSetParameterPtr _mPluginsToLoad;

  //! List of all the directories to search for plugins.
  cedar::aux::StringVectorParameterPtr _mPluginSearchPaths;

private:
  // none yet

}; // class cedar::aux::Settings

CEDAR_AUX_SINGLETON(Settings);

#endif // CEDAR_AUX_SETTINGS_H
