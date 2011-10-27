/*======================================================================================================================

    Copyright 2011 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        DataPlotter.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 14

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/gui/DataPlotter.h"
#include "auxiliaries/DataTemplate.h"
#include "auxiliaries/ImageData.h"
#include "auxiliaries/gui/MatrixPlot.h"
#include "auxiliaries/gui/ImagePlot.h"
#include "auxiliaries/gui/HistoryPlot.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QVBoxLayout>

cedar::proc::gui::DataPlotter::WidgetFactory cedar::proc::gui::DataPlotter::mTypePlotters;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::DataPlotter::DataPlotter(const std::string& title, QWidget *pParent)
:
QDockWidget(title.c_str(), pParent)
{
  this->setFloating(true);
  this->layout()->setContentsMargins(0, 0, 0, 0);
}

cedar::proc::gui::DataPlotter::~DataPlotter()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::gui::DataPlotter::plot(cedar::aux::DataPtr data)
{
  mData = data;
  //!@todo doesn't work this way -- related to the to-do entry below.
//  PlotWidgetInterface *p_widget = getWidgetFactory().get(data)->allocateRaw();
//  p_widget->plot(data);

  //!@todo find a better solution for this!
  cedar::aux::gui::DataPlotInterface *p_plot = NULL;
  if (dynamic_cast<cedar::aux::MatData*>(data.get()))
  {
    p_plot = new cedar::aux::gui::MatrixPlot(this);
  }
  else if (dynamic_cast<cedar::aux::ImageData*>(data.get()))
  {
    p_plot = new cedar::aux::gui::ImagePlot(this);
  }
  else if (dynamic_cast<cedar::aux::DoubleData*>(data.get()))
  {
    p_plot = new cedar::aux::gui::HistoryPlot(this);
  }
  else
  {
    CEDAR_THROW(cedar::aux::UnhandledTypeException, "Unhandled data type in cedar::proc::gui::DataPlotter::plot.");
  }
  connect(p_plot, SIGNAL(dataChanged()), this, SLOT(dataChanged()));
  p_plot->display(data);
  this->setWidget(p_plot);
}

cedar::proc::gui::DataPlotter::WidgetFactory& cedar::proc::gui::DataPlotter::getWidgetFactory()
{
  if (cedar::proc::gui::DataPlotter::mTypePlotters.empty())
  {
    cedar::proc::gui::DataPlotter::mTypePlotters.add<cedar::aux::MatData, QWidget>();
  }
  return cedar::proc::gui::DataPlotter::mTypePlotters;
}

void cedar::proc::gui::DataPlotter::dataChanged()
{
  this->plot(mData);
}
