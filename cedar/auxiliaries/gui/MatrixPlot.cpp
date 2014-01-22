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

    File:        MatrixPlot.cpp

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

#define NOMINMAX // prevents MSVC conflicts

// CEDAR INCLUDES
#include "cedar/configuration.h"
#include "cedar/auxiliaries/gui/MatrixPlot.h"
#include "cedar/auxiliaries/gui/ColorValueRGBA.h"
#ifdef CEDAR_USE_QWT
  #include "cedar/auxiliaries/gui/QwtLinePlot.h"
  #include "cedar/auxiliaries/gui/HistoryPlot0D.h"
#endif // CEDAR_USE_QWT
#ifdef CEDAR_USE_QWTPLOT3D
  #include "cedar/auxiliaries/gui/QwtSurfacePlot.h"
#else // CEDAR_USE_QWTPLOT3D
  #include "cedar/auxiliaries/gui/ImagePlot.h"
#endif // CEDAR_USE_QWTPLOT3D
#ifdef CEDAR_USE_VTK
#include "cedar/auxiliaries/gui/VtkLinePlot.h"
#include "cedar/auxiliaries/gui/VtkSurfacePlot.h"
#endif // CEDAR_USE_VTK
#include "cedar/auxiliaries/gui/MatrixSlicePlot3D.h"
#include "cedar/auxiliaries/gui/exceptions.h"
#include "cedar/auxiliaries/gui/PlotManager.h"
#include "cedar/auxiliaries/gui/PlotDeclaration.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/MatData.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/stringFunctions.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <iostream>
#include <limits>

//----------------------------------------------------------------------------------------------------------------------
// type registration
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registerPlot()
  {
    using cedar::aux::MatData;
    using cedar::aux::gui::MatrixPlot;

    typedef cedar::aux::gui::PlotDeclarationTemplate<MatData, MatrixPlot> DeclarationType;

    boost::shared_ptr<DeclarationType> declaration(new DeclarationType());
    declaration->declare();

    return true;
  }

  bool registered = registerPlot();
}

//----------------------------------------------------------------------------------------------------------------------
// static members
//----------------------------------------------------------------------------------------------------------------------
std::vector<cedar::aux::gui::ColorValueRGBA> cedar::aux::gui::MatrixPlot::mStandardColorVector;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::MatrixPlot::MatrixPlot(QWidget *pParent)
:
cedar::aux::gui::MultiPlotInterface(pParent),
mpCurrentPlotWidget(NULL),
mTitle("")
{
  QVBoxLayout *p_layout = new QVBoxLayout();
  this->setLayout(p_layout);

  this->setContentsMargins(0, 0, 0, 0);
  p_layout->setContentsMargins(0, 0, 0, 0);
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool cedar::aux::gui::MatrixPlot::canAppend(cedar::aux::ConstDataPtr data) const
{
  if (this->mpCurrentPlotWidget == NULL)
  {
    return false;
  }
  else if
  (
    cedar::aux::gui::MultiPlotInterface* p_multi_plot
      = dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(this->mpCurrentPlotWidget)
  )
  {
    return p_multi_plot->canAppend(data);
  }
  else
  {
    return false;
  }
}

void cedar::aux::gui::MatrixPlot::doAppend(cedar::aux::ConstDataPtr data, const std::string& title)
{
  CEDAR_DEBUG_ASSERT(this->mpCurrentPlotWidget != NULL);
  cedar::aux::gui::MultiPlotInterface *p_multi_plot
    = dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(this->mpCurrentPlotWidget);

  CEDAR_DEBUG_ASSERT(p_multi_plot != NULL);
  p_multi_plot->append(data, title);
  mTitle = title;
}

void cedar::aux::gui::MatrixPlot::plot(cedar::aux::ConstDataPtr data, const std::string& title)
{
  this->mData= boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(data);
  if (!this->mData)
  {
    CEDAR_THROW(cedar::aux::gui::InvalidPlotData,
                "Cannot cast to cedar::aux::MatData in cedar::aux::gui::MatrixPlot::plot.");
  }

  if (this->mpCurrentPlotWidget)
  {
    delete this->mpCurrentPlotWidget;
    this->mpCurrentPlotWidget = NULL;
  }

  const cv::Mat& mat = this->mData->getData();
  unsigned int dims = cedar::aux::math::getDimensionalityOf(mat);
  if (mat.empty())
  {
    dims = UINT_MAX;
  }

  switch (dims)
  {
#ifdef CEDAR_USE_QWT
    case 0:
      this->mpCurrentPlotWidget = new cedar::aux::gui::HistoryPlot0D(this->mData, title);
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;

    case 1:
      this->mpCurrentPlotWidget = new cedar::aux::gui::QwtLinePlot(this->mData, title);
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;
#elif defined CEDAR_USE_VTK
    case 1:
      this->mpCurrentPlotWidget = new cedar::aux::gui::VtkLinePlot(this->mData, title);
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;
#endif // CEDAR_USE_QWT

    case 2:
#ifdef CEDAR_USE_QWTPLOT3D
      this->mpCurrentPlotWidget = new cedar::aux::gui::QwtSurfacePlot(this->mData, title);
#elif defined CEDAR_USE_VTK
      this->mpCurrentPlotWidget = new cedar::aux::gui::VtkSurfacePlot(this->mData, title);
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;
#else
      this->mpCurrentPlotWidget = new cedar::aux::gui::ImagePlot(this->mData, title);
#endif // CEDAR_USE_QWTPLOT3D
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;
    case 3:
    {
      this->mpCurrentPlotWidget = new cedar::aux::gui::MatrixSlicePlot3D(this->mData, title);
      connect(this->mpCurrentPlotWidget, SIGNAL(dataChanged()), this, SLOT(processChangedData()));
      break;
    }

    case UINT_MAX:
    {
      std::string message = "The matrix plot widget can not handle empty matrices.";
      message += "\nPress here to refresh the plot after you have changed the dimensionality.";
      this->mpCurrentPlotWidget = new QPushButton(QString::fromStdString(message));
      connect(this->mpCurrentPlotWidget, SIGNAL(pressed()), this, SLOT(processChangedData()));
      break;
    }

    default:
    {
      std::string message = "The matrix plot widget can not handle a matrix with the given dimensionality (";
      message += cedar::aux::toString(mat.dims);
      message += ").\nPress here to refresh the plot after you have changed the dimensionality.";
      this->mpCurrentPlotWidget = new QPushButton(QString::fromStdString(message));
      connect(this->mpCurrentPlotWidget, SIGNAL(pressed()), this, SLOT(processChangedData()));
    }
  }
  this->layout()->addWidget(this->mpCurrentPlotWidget);
  mTitle = title;
}

const std::vector<cedar::aux::gui::ColorValueRGBA>& cedar::aux::gui::MatrixPlot::getStandardColorVector()
{
  if (cedar::aux::gui::MatrixPlot::mStandardColorVector.empty())
  {
    cedar::aux::gui::ColorValueRGBA rgb;
    rgb.alpha = 1;
    for (double i = 0; i < 256; i++)
    {
      if(i < 32.0)
      {
        rgb.red = 0.0;
        rgb.green = 0.0;
        rgb.blue = 0.5 + 0.5 * i/32.0;
      }
      else if(i < 96.0)
      {
        rgb.red = 0.0;
        rgb.green = (i - 32.0) / 64.0;
        rgb.blue = 1;
      }
      else if(i < 160.0)
      {
        rgb.red = (i - 96.0) / 64.0;
        rgb.green = 1.0;
        rgb.blue = 1.0 - (i - 96.0) / 64.0;
      }
      else if(i < 224.0)
      {
        rgb.red = 1.0;
        rgb.green = 1.0 - (i - 160.0) / 64.0;
        rgb.blue = 0.0;
      }
      else if(i < 256.0)
      {
        rgb.red = 1.0 - (i - 224.0) / 64.0;
        rgb.green = 0.0;
        rgb.blue = 0.0;
      }
      cedar::aux::gui::MatrixPlot::mStandardColorVector.push_back(rgb);
    }
  }

  return cedar::aux::gui::MatrixPlot::mStandardColorVector;
}

void cedar::aux::gui::MatrixPlot::processChangedData()
{
  if (mpCurrentPlotWidget)
  {
    if
    (
      cedar::aux::gui::MultiPlotInterface* p_multi
        = dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(mpCurrentPlotWidget)
    )
    {
      // first, recover data from multiplot
      cedar::aux::gui::MultiPlotInterface::DataMap map = p_multi->getDataMap();
      auto iter = map.begin();
      if (iter != map.end())
      {
        this->plot(iter->first, iter->second);
        if (dynamic_cast<cedar::aux::gui::MultiPlotInterface*>(mpCurrentPlotWidget))
        {
          for (auto rest = ++iter; rest != map.end(); ++rest)
          {
            this->append(rest->first, rest->second);
          }
        }
      }
    }
    else
    {
      this->plot(this->mData, mTitle);
    }
  }
}
