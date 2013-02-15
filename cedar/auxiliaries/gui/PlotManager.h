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

    File:        PlotManager.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 03 26

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GUI_PLOT_MANAGER_H
#define CEDAR_AUX_GUI_PLOT_MANAGER_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/namespace.h"
#include "cedar/auxiliaries/gui/PlotDeclaration.h"
#include "cedar/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/stringFunctions.h"
#include "cedar/auxiliaries/utilities.h"

// SYSTEM INCLUDES
#include <set>

/*!@brief A manager for different types of plots
 */
class cedar::aux::gui::PlotManager
{
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief produces a vector of suitable plt classes for given data
  void getPlotClassesFor(cedar::aux::ConstDataPtr data, std::set<cedar::aux::gui::PlotDeclarationPtr>& declarations);

  //!@brief returns the default declaration for given data
  cedar::aux::gui::PlotDeclarationPtr getDefaultDeclarationFor(cedar::aux::ConstDataPtr data);

  //!@brief declare a plot type
  void declare(cedar::aux::gui::PlotDeclarationPtr declaration);

  //!@brief set default
  template <typename DataType, typename PlotterType>
  void setDefault()
  {
    std::string data_type = cedar::aux::typeToString<DataType>();
    std::string plotter_type = cedar::aux::typeToString<PlotterType>();
    cedar::aux::gui::PlotManager::normalizeTypeName(data_type);
    cedar::aux::gui::PlotManager::normalizeTypeName(plotter_type);

    mDefaultPlots[data_type] = plotter_type;
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
  static void normalizeTypeName(std::string& typeName)
  {
    typeName = cedar::aux::replace(typeName, "::", ".");
  }

  static std::string getNormalizedTypeName(const std::string& typeName)
  {
    std::string ret = typeName;
    normalizeTypeName(ret);
    return ret;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //! Mapping from data types to default plot types.
  std::map<std::string, std::string> mDefaultPlots;

  //! Mapping from plot type strings to declarations
  std::map<std::string, cedar::aux::gui::PlotDeclarationPtr> mPlotTypeDeclarations;

}; // class cedar::aux::gui::PlotManager

namespace cedar
{
  namespace aux
  {
    namespace gui
    {
      /*!@brief singleton specialization for the plot manager
       * @todo can this be moved to namespace.h?
       */
      typedef cedar::aux::Singleton<cedar::aux::gui::PlotManager> PlotManagerSingleton;
    }
  }
}

#endif // CEDAR_AUX_GUI_PLOT_MANAGER_H

