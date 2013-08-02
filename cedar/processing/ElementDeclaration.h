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
#include <QIcon>
#include <QResource>
#include <vector>


/*!@brief A StepDeclaration contains the relation of a unique class id (as string) and the corresponding factory to
 * create a step of this id. It is a concretization of DeclarationBase.
 */
class cedar::proc::ElementDeclaration : public cedar::aux::PluginDeclarationBaseTemplate<cedar::proc::ElementPtr>
{
public:
  //! Holds information about data in a custom step plot.
  struct PlotData
  {
    /*! Constructor.
     *
     * @param id Role of the data to be plotted.
     * @param name Name of the data slot of the step whose data is to be plotted.
     * @param ignoreIfMissing If true, no warning will be generated if the slot is missing.
     */
    PlotData
    (
      cedar::proc::DataRole::Id id = cedar::proc::DataRole::OUTPUT,
      const std::string& name = "",
      bool ignoreIfMissing = false
    )
    :
    mId(id),
    mName(name),
    mIgnoreIfMissing(ignoreIfMissing)
    {
    }

    //! Role of the data to be plotted.
    cedar::proc::DataRole::Id mId;

    //! Name of the data to be plotted.
    std::string mName;

    //! If true, no exception will be thrown if the data cannot be found.
    bool mIgnoreIfMissing;
  };

  //!@brief list that pairs a data role with the desired plot
  typedef std::vector<PlotData> DataList;

  /*!@brief Structure that holds information about custom plots defined for a processing step.
   */
  struct PlotDefinition
  {
    /*! Constructor
     *
     * @param name Name of the plot to be displayed in the "defined plots" menu.
     * @param icon (Optional) path to the icon to be used for the plot in the "defined plots" menu.
     */
    PlotDefinition(const std::string& name, const std::string& icon = std::string())
    :
    mName(name),
    mIcon(icon)
    {
    }

    //! Appends data to the list of items to plot.
    void appendData(cedar::proc::DataRole::Id id, const std::string& dataName, bool ignoreIfMissing = false)
    {
      this->mData.push_back(PlotData(id, dataName, ignoreIfMissing));
    }

    //! Name of the plot.
    std::string mName;

    //! Icon of the plot.
    std::string mIcon;

    //! Data to be plotted.
    DataList mData;
  };

  //!@brief list of plot definitions
  typedef std::vector<PlotDefinition> PlotDefinitionList;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //! Constructor that takes a category and an (optional) class name to use for the element.
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

  //!@brief Returns the actual icon for the element.
  QIcon getIcon() const
  {
    return QIcon(this->determinedIconPath());
  }

  /*! Returns the path for the icon to use; this will also return special icons if there is an error with the specified
   * icons.
   */
  QString determinedIconPath() const
  {
    QResource existance_test(QString::fromStdString(this->getIconPath()));
    if (existance_test.isValid())
    {
      auto icon = QIcon(QString::fromStdString(this->getIconPath()));
      if (icon.isNull())
      {
        return ":/steps/no_icon.svg";
      }
      else
      {
        return QString::fromStdString(this->getIconPath());
      }
    }
    else
    {
      return ":/steps/broken_icon.svg";
    }
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

  /*!@brief Defines a new plot for this type of element
   *
   * @param plotDefinition Definition of the plot.
   */
  void definePlot(const PlotDefinition& plotDefinition)
  {
    this->mPlots.push_back(plotDefinition);
  }

  //!@brief Returns the defined plots for this declaration.
  const PlotDefinitionList& definedPlots() const
  {
    return this->mPlots;
  }

  //! Overriden in the templated version by the super class implementation.
  virtual void declare() const = 0;

  //! Set the default plot. When the plot is an empty string, "plot all" will be the default.
  void setDefaultPlot(const std::string& plotName)
  {
    this->mDefaultPlot = plotName;
  }

  //! Returns the name of the default plot. Empty if none is set.
  const std::string& getDefaultPlot() const
  {
    return this->mDefaultPlot;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  void customRead(const cedar::aux::ConfigurationNode& node)
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

  //! Default plot to open (if empty, all data is plotted).
  std::string mDefaultPlot;
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
