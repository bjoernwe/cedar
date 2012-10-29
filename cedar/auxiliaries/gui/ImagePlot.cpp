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

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/ImagePlot.h"
#include "cedar/auxiliaries/gui/MatrixPlot.h" // for the color map
#include "cedar/auxiliaries/gui/PlotManager.h"
#include "cedar/auxiliaries/annotation/ColorSpace.h"
#include "cedar/auxiliaries/assert.h"
#include "cedar/auxiliaries/gui/exceptions.h"
#include "cedar/auxiliaries/ImageData.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <QToolTip>
#include <QMouseEvent>
#include <QThread>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// type registration
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registerPlot()
  {
    typedef cedar::aux::gui::PlotDeclarationTemplate<cedar::aux::MatData, cedar::aux::gui::ImagePlot> DeclarationTypeM;
    boost::shared_ptr<DeclarationTypeM> declaration(new DeclarationTypeM());
    cedar::aux::gui::PlotManagerSingleton::getInstance()->declare(declaration);

    return true;
  }

  bool registered = registerPlot();
}

//----------------------------------------------------------------------------------------------------------------------
// static members
//----------------------------------------------------------------------------------------------------------------------

std::vector<char> cedar::aux::gui::ImagePlot::mLookupTableR;
std::vector<char> cedar::aux::gui::ImagePlot::mLookupTableG;
std::vector<char> cedar::aux::gui::ImagePlot::mLookupTableB;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::gui::ImagePlot::ImagePlot(QWidget *pParent)
:
cedar::aux::gui::PlotInterface(pParent),
mTimerId(0),
mDataType(DATA_TYPE_UNKNOWN),
mConverting(false)
{
  QVBoxLayout *p_layout = new QVBoxLayout();
  p_layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(p_layout);
  this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

  mpImageDisplay = new cedar::aux::gui::ImagePlot::ImageDisplay("no image loaded");
  p_layout->addWidget(mpImageDisplay);

  this->mpWorkerThread = new QThread();
  mWorker = cedar::aux::gui::detail::ImagePlotWorkerPtr(new cedar::aux::gui::detail::ImagePlotWorker(this));
  mWorker->moveToThread(this->mpWorkerThread);

  QObject::connect(this, SIGNAL(convert()), mWorker.get(), SLOT(convert()));
  QObject::connect(mWorker.get(), SIGNAL(done()), this, SLOT(conversionDone()));

  this->mpWorkerThread->start(QThread::LowPriority);
}

cedar::aux::gui::ImagePlot::ImageDisplay::ImageDisplay(const QString& text)
:
QLabel(text)
{
  this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  this->setWordWrap(true);
}

cedar::aux::gui::ImagePlot::~ImagePlot()
{
  if (this->mpWorkerThread)
  {
    this->mpWorkerThread->quit();
    this->mpWorkerThread->wait();
    delete this->mpWorkerThread;
    this->mpWorkerThread = NULL;
  }
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::gui::ImagePlot::ImageDisplay::mousePressEvent(QMouseEvent * pEvent)
{
  //!@todo Use the channel annotation for values from three channel images
  if (!this->pixmap() || !this->mData)
    return;

  const cv::Mat& matrix = this->mData->getData();

  if (matrix.empty())
  {
    return;
  }

  int label_x = pEvent->x();
  int label_y = pEvent->y();
  int image_x = label_x - (this->width() - this->pixmap()->width()) / 2;
  int image_y = label_y - (this->height() - this->pixmap()->height()) / 2;

  double rel_x = static_cast<double>(image_x) / static_cast<double>(this->pixmap()->width());
  double rel_y = static_cast<double>(image_y) / static_cast<double>(this->pixmap()->height());
  int x = static_cast<int>(rel_x * static_cast<double>(matrix.cols));
  int y = static_cast<int>(rel_y * static_cast<double>(matrix.rows));

  if (x < 0 || y < 0 || x >= matrix.cols || y >= matrix.rows)
  {
    return;
  }

  QString info_text;
  info_text += QString("x, y = %1, %2").arg(x).arg(y);

  info_text += "<br />value: ";
  switch (matrix.channels())
  {
    case 1:
      info_text += QString("%1").arg(cedar::aux::math::getMatrixEntry<double>(matrix, y, x));
      break;

    case 3:
      switch(matrix.depth())
      {
        case CV_8U:
        {
          const cv::Vec3b& value = matrix.at<cv::Vec3b>(y, x);
          info_text += QString("%1, %2, %3").arg(static_cast<int>(value[0]))
                                            .arg(static_cast<int>(value[1]))
                                            .arg(static_cast<int>(value[2]));
          break;
        }

        case CV_8S:
        {
          const cv::Vec3s& value = matrix.at<cv::Vec3s>(y, x);
          info_text += QString("%1, %2, %3").arg(static_cast<int>(value[0]))
                                            .arg(static_cast<int>(value[1]))
                                            .arg(static_cast<int>(value[2]));
          break;
        }

        default:
          QToolTip::showText(pEvent->globalPos(), QString("Matrix depth (%1) not handled.").arg(matrix.depth()));
          return;
      }
      break;

    default:
      // should never happen, all cases should be handled above
      QToolTip::showText(pEvent->globalPos(), QString("Matrix channel count (%1) not handled.").arg(matrix.channels()));
      return;
  }

  QToolTip::showText(pEvent->globalPos(), info_text);
}

//!@cond SKIPPED_DOCUMENTATION
void cedar::aux::gui::detail::ImagePlotWorker::convert()
{
  const cv::Mat& mat = this->mpPlot->mData->getData();

  this->mpPlot->mData->lockForRead();
  if (mat.empty())
  {
    this->mpPlot->mpImageDisplay->setText("Matrix is empty.");
    this->mpPlot->mData->unlock();
    return;
  }
  int type = mat.type();
  this->mpPlot->mData->unlock();

  switch(type)
  {
    case CV_8UC1:
    {
      this->mpPlot->mData->lockForRead();
      cv::Mat converted = this->mpPlot->threeChannelGrayscale(this->mpPlot->mData->getData());
      this->mpPlot->mData->unlock();
      CEDAR_DEBUG_ASSERT(converted.type() == CV_8UC3);
      this->mpPlot->imageFromMat(converted);
      break;
    }

    case CV_8UC3:
    {
      this->mpPlot->mData->lockForRead();
      this->mpPlot->imageFromMat(this->mpPlot->mData->getData());
      this->mpPlot->mData->unlock();
      break;
    }

    case CV_32FC1:
    {
      //!@todo This should color-code when the data is not an image (i.e., a matrix)
      //!@todo Some code here is redundant
      // find min and max for scaling
      double min, max;

      this->mpPlot->mData->lockForRead();
      cv::minMaxLoc(mat, &min, &max);
      cv::Mat scaled = (mat - min) / (max - min) * 255.0;
      this->mpPlot->mData->unlock();
      cv::Mat mat_8u;
      scaled.convertTo(mat_8u, CV_8U);

      // convert grayscale to three-channel matrix
      cv::Mat converted = this->mpPlot->threeChannelGrayscale(mat_8u);
      CEDAR_DEBUG_ASSERT(converted.type() == CV_8UC3);
      this->mpPlot->imageFromMat(converted);
      break;
    }

    default:
    {
      std::string matrix_type_name = cedar::aux::math::matrixTypeToString(mat);
      QString text = "Cannot display matrix of type " + QString::fromStdString(matrix_type_name) + ".";
      this->mpPlot->mpImageDisplay->setText(text);
      return;
    }
  }

  emit done();
}
//!@endcond

void cedar::aux::gui::ImagePlot::conversionDone()
{
  this->mpImageDisplay->setPixmap(QPixmap::fromImage(this->mImage));
  this->resizePixmap();
  mConverting = false;
}

void cedar::aux::gui::ImagePlot::timerEvent(QTimerEvent * /*pEvent*/)
{
  if
  (
    !this->isVisible() // plot widget is not visible -- no need to plot
    || this->mDataType == DATA_TYPE_UNKNOWN // data type cannot be plotted
    || mConverting // already converting -- skip this timer event
  )
  {
    return;
  }

  mConverting = true;
  emit convert();
}

//!@todo encapsulate lookup table functionality in own class
cv::Mat cedar::aux::gui::ImagePlot::threeChannelGrayscale(const cv::Mat& in) const
{
  CEDAR_DEBUG_ASSERT(in.channels() == 1);

  if (mLookupTableR.empty() || mLookupTableG.empty() || mLookupTableB.empty())
  {
    const size_t steps = 256;
    const Qwt3D::ColorVector& standard = cedar::aux::gui::MatrixPlot::getStandardColorVector();
    mLookupTableR.resize(steps, 0);
    mLookupTableG.resize(steps, 0);
    mLookupTableB.resize(steps, 0);

    for (size_t v = 0; v < steps; ++v)
    {
      char& r = mLookupTableR.at(v);
      char& g = mLookupTableG.at(v);
      char& b = mLookupTableB.at(v);

      size_t closest_standard = (v * standard.size()) / steps;
      size_t lower_closest = (closest_standard * steps) / standard.size();
      size_t upper_closest = ((closest_standard + 1) * steps) / standard.size();

      CEDAR_DEBUG_ASSERT(closest_standard < standard.size());

      double r_lower = static_cast<double>(standard.at(closest_standard).r) * 255.0;
      double g_lower = static_cast<double>(standard.at(closest_standard).g) * 255.0;
      double b_lower = static_cast<double>(standard.at(closest_standard).b) * 255.0;
      if (closest_standard + 1 < standard.size())
      {
        double r_upper = static_cast<double>(standard.at(closest_standard + 1).r);
        double g_upper = static_cast<double>(standard.at(closest_standard + 1).g);
        double b_upper = static_cast<double>(standard.at(closest_standard + 1).b);

        double factor = static_cast<double>(v - lower_closest) / static_cast<double>(upper_closest - lower_closest);

        double d_r = (1.0 - factor) * r_lower + factor * r_upper;
        double d_g = (1.0 - factor) * g_lower + factor * g_upper;
        double d_b = (1.0 - factor) * b_lower + factor * b_upper;
        r = static_cast<char>(d_r);
        g = static_cast<char>(d_g);
        b = static_cast<char>(d_b);
      }
      else
      {
        r = r_lower;
        g = g_lower;
        b = b_lower;
      }
    }
  }

  switch (this->mDataType)
  {
    default:
    case DATA_TYPE_IMAGE:
    {
      cedar::aux::annotation::ColorSpace::ChannelType type = cedar::aux::annotation::ColorSpace::Gray;

      if (this->mDataColorSpace)
      {
        type = this->mDataColorSpace->getChannelType(0);
      }

      if (type == cedar::aux::annotation::ColorSpace::Hue)
      {
        cv::Mat ones = 0.0 * in + 255.0;
        std::vector<cv::Mat> merge_vec;
        merge_vec.push_back(in);
        merge_vec.push_back(ones);
        merge_vec.push_back(ones);
        cv::Mat merged, converted;
        cv::merge(merge_vec, merged);
        cv::cvtColor(merged, converted, CV_HSV2BGR);
        return converted;
      }
      else
      {
        std::vector<cv::Mat> merge_vec;
        cv::Mat zeros = 0.0 * in;

        switch (type)
        {
          case cedar::aux::annotation::ColorSpace::Red:
            merge_vec.push_back(zeros);
            merge_vec.push_back(zeros);
            merge_vec.push_back(in);
            break;

          case cedar::aux::annotation::ColorSpace::Green:
            merge_vec.push_back(zeros);
            merge_vec.push_back(in);
            merge_vec.push_back(zeros);
            break;

          case cedar::aux::annotation::ColorSpace::Blue:
            merge_vec.push_back(in);
            merge_vec.push_back(zeros);
            merge_vec.push_back(zeros);
            break;

          default:
          case cedar::aux::annotation::ColorSpace::Gray:
            merge_vec.push_back(in);
            merge_vec.push_back(in);
            merge_vec.push_back(in);
            break;
        }

        cv::Mat converted;
        cv::merge(merge_vec, converted);
        return converted;
      }
    }

    case DATA_TYPE_MAT:
    {
      // accept only char type matrices
      CEDAR_ASSERT(in.depth() != sizeof(char));

      int channels = in.channels();

      int rows = in.rows * channels;
      int cols = in.cols;

      if (in.isContinuous())
      {
          cols *= rows;
          rows = 1;
      }

      cv::Mat converted = cv::Mat(in.rows, in.cols, CV_8UC3);

      int i,j;
      const unsigned char* p_in;
      unsigned char* p_converted;
      for( i = 0; i < rows; ++i)
      {
          p_in = in.ptr<unsigned char>(i);
          p_converted = converted.ptr<unsigned char>(i);
          for ( j = 0; j < cols; ++j)
          {
            size_t v = static_cast<size_t>(p_in[j]);
            // channel 0
            p_converted[3 * j + 0] = mLookupTableB.at(v);
            // channel 1
            p_converted[3 * j + 1] = mLookupTableG.at(v);
            // channel 2
            p_converted[3 * j + 2] = mLookupTableR.at(v);
          }
      }
      return converted;
    }
  }
}

void cedar::aux::gui::ImagePlot::imageFromMat(const cv::Mat& mat)
{
  this->mImage = QImage
  (
    mat.data,
    mat.cols,
    mat.rows,
    mat.step,
    QImage::Format_RGB888
  ).rgbSwapped();
}

void cedar::aux::gui::ImagePlot::plot(cedar::aux::ConstDataPtr data, const std::string& /* title */)
{
  if (mTimerId != 0)
    this->killTimer(mTimerId);

  mDataType = DATA_TYPE_MAT;

  this->mData = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(data);
  this->mpImageDisplay->mData = this->mData;
  if (!this->mData)
  {
    CEDAR_THROW(cedar::aux::gui::InvalidPlotData,
                "Cannot cast to cedar::aux::MatData in cedar::aux::gui::ImagePlot::plot.");
  }

  this->mDataColorSpace.reset();
  try
  {
    this->mDataColorSpace = this->mData->getAnnotation<cedar::aux::annotation::ColorSpace>();
    mDataType = DATA_TYPE_IMAGE;
  }
  catch (cedar::aux::AnnotationNotFoundException&)
  {
    // ok, not an image
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
