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

    File:        ElementDeclaration.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 11 18

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_ELEMENT_DECLARATION_H
#define CEDAR_PROC_ELEMENT_DECLARATION_H

// CEDAR INCLUDES
#include "cedar/processing/namespace.h"
#include "cedar/processing/DataRole.h"
#include "cedar/auxiliaries/utilities.h"
#include "cedar/auxiliaries/stringFunctions.h"
#include "cedar/auxiliaries/FactoryDerived.h"
#include "cedar/auxiliaries/PluginDeclaration.h"
#include "cedar/auxiliaries/PluginDeclarationTemplate.h"

// SYSTEM INCLUDES
#include <vector>


/*!@brief A StepDeclaration contains the relation of a unique class id (as string) and the corresponding factory to
 * create a step of this id. It is a concretization of DeclarationBase.
 * @todo With the revised factory, passing the factory type is probably unnecessary
 */
class cedar::proc::ElementDeclaration : public cedar::aux::PluginDeclarationBaseTemplate<cedar::proc::ElementPtr>
{
public:
  //!@brief list that pairs a data role with the desired plot
  typedef std::vector<std::pair<cedar::proc::DataRole::Id, std::string> > DataList;
  //!@brief list of plot definitions
  typedef std::vector<std::pair<std::string, DataList> > PlotDefinitionList;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  ElementDeclaration(const std::string& category, const std::string& className = "")
  :
  cedar::aux::PluginDeclarationBaseTemplate<cedar::proc::ElementPtr>(category, className)
  {
  }

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief set path to icon included in the graphical representation of this element
  void setIconPath(const std::string& path)
  {
    this->mIconPath = path;
  }

  //!@brief get path to icon included in the graphical representation of this element
  const std::string& getIconPath() const
  {
    return this->mIconPath;
  }

  //!@brief Method for setting the description of the element.
  void setDescription(const std::string& description)
  {
    this->mDescription = description;
  }

  //!@brief Returns the description of the element.
  const std::string& getDescription() const
  {
    return this->mDescription;
  }

  /*!@brief Reads some values such as the icon path from the configuration node.
   */
  virtual void read(const cedar::aux::ConfigurationNode& node)
  {
    if (node.find("icon") != node.not_found())
    {
      this->setIconPath(node.get_child("icon").get_value<std::string>());
    }

    if (node.find("description") != node.not_found())
    {
      this->setDescription(node.get_child("description").get_value<std::string>());
    }
  }

  /*!@brief Defines a new plot for this type of element
   *
   * @param plotName    Name of the plot, displayed in the UI.
   * @param slotsToPlot  List of data slots to plot.
   *
   * @todo  This should also be read from the plugin xml file.
   */
  void definePlot(const std::string& plotName, const DataList& slotsToPlot)
  {
    this->mPlots.push_back(std::make_pair(plotName, slotsToPlot));
  }

  //!@brief Returns the defined plots for this declaration.
  const PlotDefinitionList& definedPlots() const
  {
    return this->mPlots;
  }

  //! Overriden in the templated version by the super class implementation.
  virtual void declare() const = 0;

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
  //!@brief path to icon included in the graphical representation of this step
  std::string mIconPath;

  //!@brief The description of the class.
  std::string mDescription;

  //!@brief Declarations of plots.
  PlotDefinitionList mPlots;
};


/*!@brief This is a template class for comfortably generating element declarations.
 */
template <class DerivedClass>
class cedar::proc::ElementDeclarationTemplate
    :
    public cedar::aux::PluginDeclarationTemplate
    <
      cedar::proc::ElementPtr,
      boost::shared_ptr<DerivedClass>,
      cedar::proc::ElementDeclaration
    >
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
private:
  typedef
      cedar::aux::PluginDeclarationTemplate<cedar::proc::ElementPtr, boost::shared_ptr<DerivedClass>, cedar::proc::ElementDeclaration>
      DeclarationSuper;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief The constructor.
   *
   * @param category Category for the element.
   * @param classId  Identifier of the class. If this is left empty, the name will be determined automatically. For
   *                 example, a class test::namespaceName::ClassName will result in the name
   *                 test.namespaceName.ClassName.
   */
  ElementDeclarationTemplate(const std::string& category, const std::string& classId = "")
  :
  DeclarationSuper
  (
    category,
    classId
  )
  {
  }

  void declare() const
  {
    DeclarationSuper::declare();
  }
}; // class cedar::proc::ElementDeclarationTemplate

#endif // CEDAR_PROC_ELEMENT_DECLARATION_H
