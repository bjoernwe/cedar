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

// LOCAL INCLUDES
#include "auxiliaries/gui/ImagePlot.h"
#include "auxiliaries/macros.h"
#include "auxiliaries/gui/exceptions.h"
#include "auxiliaries/ImageData.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::ImagePlot::ImagePlot(QWidget *pParent)
:
cedar::aux::gui::DataPlotInterface(pParent),
mTimerId(0)
{
  QVBoxLayout *p_layout = new QVBoxLayout();
  p_layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(p_layout);
  this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  mpImageDisplay = new QLabel("no image loaded");
  p_layout->addWidget(mpImageDisplay);
  mpImageDisplay->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

cedar::aux::gui::ImagePlot::~ImagePlot()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::gui::ImagePlot::timerEvent(QTimerEvent */*pEvent*/)
{
  this->mData->lockForRead();
  cv::Mat& mat = this->mData->getData();
  switch(mat.type())
  {
    case CV_8U:
      this->mImage = QImage
      (
        mat.cols,
        mat.rows,
        QImage::Format_RGB888
      );

      for (int row = 0; row < mat.rows; ++row)
      {
        for (int column = 0; column < mat.cols; ++column)
        {
          uint8_t grey = mat.at<uint8_t>(row, column);
          uint rgb = grey + (grey << 8) + (grey << 16);
          this->mImage.setPixel(column, row, rgb);
        }
      }

      break;

    case CV_8UC3:
    {
      this->mImage = QImage
                      (
                        mat.data,
                        mat.cols,
                        mat.rows,
                        mat.step,
                        QImage::Format_RGB888
                      ).rgbSwapped();
      break;
    }

    case CV_32F:
      std::cout << "Unhandled cv::Mat type CV_32F in cedar::aux::gui::ImagePlot::update()." << std::endl;
      break;

    case CV_32FC3:
      std::cout << "Unhandled cv::Mat type CV_32FC3 in cedar::aux::gui::ImagePlot::update()." << std::endl;
      break;

    case CV_64F:
      std::cout << "Unhandled cv::Mat type CV_64F in cedar::aux::gui::ImagePlot::update()." << std::endl;
      break;

    default:
      std::cout << "Unhandled cv::Mat type " << mat.type()
                << " in cedar::aux::gui::ImagePlot::update()." << std::endl;
  }

  this->mpImageDisplay->setPixmap(QPixmap::fromImage(this->mImage));
  this->resizePixmap();
  this->mData->unlock();
}

void cedar::aux::gui::ImagePlot::display(cedar::aux::DataPtr data)
{
  if (mTimerId != 0)
    this->killTimer(mTimerId);

  this->mData = boost::shared_dynamic_cast<cedar::aux::ImageData>(data);
  if (!this->mData)
  {
    CEDAR_THROW(cedar::aux::gui::InvalidPlotData,
                "Cannot cast to cedar::aux::ImageData in cedar::aux::gui::ImagePlot::display.");
  }

  mpImageDisplay->setText("no image loaded");

  if (!this->mData->getData().empty())
  {
    mpImageDisplay->setText("");
    this->mTimerId = this->startTimer(70);
    CEDAR_DEBUG_ASSERT(mTimerId != 0);
  }
}

void cedar::aux::gui::ImagePlot::resizeEvent(QResizeEvent * /*pEvent*/)
{
  this->resizePixmap();
}

void cedar::aux::gui::ImagePlot::resizePixmap()
{
  QSize scaled_size = this->mImage.size();
  scaled_size.scale(this->mpImageDisplay->size(), Qt::KeepAspectRatio);
  if ( (!this->mpImageDisplay->pixmap()
        || scaled_size != this->mpImageDisplay->pixmap()->size()
        )
        && !this->mImage.isNull()
      )
  {
    QImage scaled_image = this->mImage.scaled(this->mpImageDisplay->size(),
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation);
    this->mpImageDisplay->setPixmap(QPixmap::fromImage(scaled_image));
  }
}
