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

    File:        Object.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 04

    Description: implementation of cedar::aux::Object class, providing geometry of a rigid object

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "Object.h"

// PROJECT INCLUDES
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES
#include <cv.h>

using namespace cv;
using namespace cedar::aux;
using namespace cedar::aux::math;

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::Object::Object()
:
mTransformation(4, 4, CV_64FC1),
mPosition(4, 1, CV_64FC1),
mOrientationQuaternion(4, 1, CV_64FC1),
mTransformationTranspose(4, 4, CV_64FC1)
{
  init();
}

cedar::aux::Object::Object(const std::string& configFileName)
:
cedar::aux::ConfigurationInterface(configFileName),
mTransformation(4, 4, CV_64FC1),
mPosition(4, 1, CV_64FC1),
mOrientationQuaternion(4, 1, CV_64FC1),
mTransformationTranspose(4, 4, CV_64FC1)
{
  init();
  addParameter(&_mPosition, "Object.position", 1.0);
  addParameter(&_mOrientation, "Object.orientation", 0.0);
  readOrDefaultConfiguration();
  mPosition.at<double>(0, 0) = _mPosition[0];
  mPosition.at<double>(1, 0) = _mPosition[1];
  mPosition.at<double>(2, 0) = _mPosition[2];
  mOrientationQuaternion.at<double>(0, 0) = _mOrientation[0];
  mOrientationQuaternion.at<double>(1, 0) = _mOrientation[1];
  mOrientationQuaternion.at<double>(2, 0) = _mOrientation[2];
  mOrientationQuaternion.at<double>(3, 0) = _mOrientation[3];
  updateTransformation();
}


cedar::aux::Object::~Object()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

cv::Mat Object::getPosition()
{
  return mPosition.clone();
}

double Object::getPositionX()
{
  return mPosition.at<double>(0, 0);
}

double Object::getPositionY()
{
  return mPosition.at<double>(1, 0);
}

double Object::getPositionZ()
{
  return mPosition.at<double>(2, 0);
}

double Object::getOrientationQuaternion(unsigned int component)
{
  return mOrientationQuaternion.at<double>(component, 0);
}

cv::Mat Object::getOrientationQuaternion()
{
  return mOrientationQuaternion.clone();
}

cv::Mat Object::getTransformation()
{
  return mTransformation.clone();
}

void Object::setPosition(double x, double y, double z)
{
  mPosition.at<double>(0, 0) = x;
  mPosition.at<double>(1, 0) = y;
  mPosition.at<double>(2, 0) = z;
  updateTransformation();
}

void Object::setPosition(const cv::Mat& position)
{
  assert(position.type() == mPosition.type());
  mPosition = position.clone();
  updateTransformation();
}

void Object::setOrientationQuaternion(cv::Mat quaternion)
{
  assert(quaternion.type() == mOrientationQuaternion.type());
  mOrientationQuaternion = quaternion.clone();
  updateTransformation();
}

void Object::setOrientationQuaternion(unsigned int component, double value)
{
  mOrientationQuaternion.at<double>(component, 0) = value;
  mOrientationQuaternion = mOrientationQuaternion * 1.0 / norm(mOrientationQuaternion);
}

void Object::updateTransformation()
{
  // now using quaternions
  double a = mOrientationQuaternion.at<double>(0, 0);
  double b = mOrientationQuaternion.at<double>(1, 0);
  double c = mOrientationQuaternion.at<double>(2, 0);
  double d = mOrientationQuaternion.at<double>(3, 0);
  mTransformation.at<double>(0, 0) = a*a + b*b - c*c - d*d;
  mTransformation.at<double>(1, 0) = 2*b*c + 2*a*d;
  mTransformation.at<double>(2, 0) = 2*b*d - 2*a*c;

  mTransformation.at<double>(0, 1) = 2*b*c - 2*a*d;
  mTransformation.at<double>(1, 1) = a*a - b*b + c*c - d*d;
  mTransformation.at<double>(2, 1) = 2*c*d + 2*a*b;

  mTransformation.at<double>(0, 2) = 2*b*d + 2*a*c;
  mTransformation.at<double>(1, 2) = 2*c*d - 2*a*b;
  mTransformation.at<double>(2, 2) = a*a - b*b - c*c + d*d;

  mTransformation.at<double>(0, 3) = mPosition.at<double>(0, 0);
  mTransformation.at<double>(1, 3) = mPosition.at<double>(1, 0);
  mTransformation.at<double>(2, 3) = mPosition.at<double>(2, 0);

  mTransformation.at<double>(3, 3) = 1;

}

void Object::init()
{
  mPosition = 0.0;
  mPosition.at<double>(3, 0) = 1.0;
  mOrientationQuaternion = 0.0;
  mOrientationQuaternion.at<double>(0, 0) = 1.0;
  mTransformation = 0.0;
  mTransformationTranspose = 0.0;
  updateTransformation();
}
