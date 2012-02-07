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
    Date:        2010 10 27

    Description: Virtual class for a simple object geometry for visualization with OpenGL

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/gl/Object.h"
#include "cedar/auxiliaries/gl/drawShapes.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gl::Object::Object(cedar::aux::RigidBodyPtr pRigidBody)
:
mpRigidBody(pRigidBody),
mTransformationTranspose(4, 4, CV_64FC1)
{
  init();
}

cedar::aux::gl::Object::~Object()
{

}

void cedar::aux::gl::Object::init()
{
  mRigidBodyType = std::string("no type");
  mIsVisible = true;
  mIsDrawnAsWireFrame = false;
  mIsDrawingLocalCoordinateFrame = false;
  mAxisLength = 1.0;
  mResolution = 10;
  mColorR = 1;
  mColorG = 0;
  mColorB = 0;
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool cedar::aux::gl::Object::isVisible()
{

  return mIsVisible;
}

std::string cedar::aux::gl::Object::getRigidBodyName()
{
  return mpRigidBody->getName();
}

std::string cedar::aux::gl::Object::getObjectType()
{
  return mRigidBodyType;
}

int cedar::aux::gl::Object::getResolution()
{
  return mResolution;
}

double cedar::aux::gl::Object::getColorR()
{
  return mColorR;
}

double cedar::aux::gl::Object::getColorG()
{
  return mColorG;
}

double cedar::aux::gl::Object::getColorB()
{
  return mColorB;
}

void cedar::aux::gl::Object::setDrawAsWireFrame(bool state)
{
  mIsDrawnAsWireFrame = state;
}

bool cedar::aux::gl::Object::isDrawnAsWireFrame()
{
  return mIsDrawnAsWireFrame;
}

void cedar::aux::gl::Object::setDrawLocalCoordinateFrame(bool state)
{
  mIsDrawingLocalCoordinateFrame = state;
}

bool cedar::aux::gl::Object::isDrawingLocalCoordinateFrame()
{
  return mIsDrawingLocalCoordinateFrame;
}

void cedar::aux::gl::Object::setAxisLength(const double value)
{
  mAxisLength = value;
}

double cedar::aux::gl::Object::getAxisLength()
{
  return mAxisLength;
}

void cedar::aux::gl::Object::setResolution(int value)
{
  mResolution = value;
}

void cedar::aux::gl::Object::setColor(double R, double G, double B)
{
  mColorR = R;
  mColorG = G;
  mColorB = B;
}

cedar::aux::RigidBodyPtr cedar::aux::gl::Object::getRigidBody()
{
  return mpRigidBody;
}

void cedar::aux::gl::Object::prepareDraw()
{
  // move to origin
  glPopMatrix();
  glPushMatrix();

  // move to object coordinates
  mTransformationTranspose = mpRigidBody->getTransformation().t();
  glMultMatrixd((GLdouble*)mTransformationTranspose.data);

  // draw local coordinate frame
  drawLocalCoordinateFrame();
}

void cedar::aux::gl::Object::drawLocalCoordinateFrame()
{
  if (mIsDrawingLocalCoordinateFrame)
  {
    cedar::aux::gl::drawAxes(mAxisLength);
  }
}

void cedar::aux::gl::Object::setVisibility(bool state)
{ 
  mIsVisible = state;
}
