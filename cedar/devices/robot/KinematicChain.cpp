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

    File:        KinematicChain.cpp

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2010 11 15

    Description: Chain of joints (e.g., a robotic arm).

    Credits:

======================================================================================================================*/


// LOCAL INCLUDES
#include "devices/robot/KinematicChain.h"
#include "devices/robot/ReferenceGeometry.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

using namespace std;
using namespace cedar::dev::robot;
using namespace cv;

//------------------------------------------------------------------------------
// constructors and destructor
//------------------------------------------------------------------------------

//! constructor
KinematicChain::KinematicChain(const cedar::dev::robot::ReferenceGeometryPtr& rpReferenceGeometry)
:
LoopedThread(50.5), //!\todo this step size should be set different, should be a parameter, i.e. read in from configuration file
mpReferenceGeometry(rpReferenceGeometry)
{
  setWorkingMode(ANGLE);
  init();
}

KinematicChain::KinematicChain(const std::string& configFileName)
:
LoopedThread(50.0, 0.001, configFileName), //!\todo this step size should be set different, should be a parameter, i.e. read in from configuration file
mpReferenceGeometry(new ReferenceGeometry(configFileName))
{
  setWorkingMode(ANGLE);
  init();
}

//! destructor
KinematicChain::~KinematicChain()
{
}

//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------

const ReferenceGeometryPtr& KinematicChain::getReferenceGeometry() const
{
  return mpReferenceGeometry;
}


unsigned int KinematicChain::getNumberOfJoints() const
{
  return mpReferenceGeometry->getNumberOfJoints();
}


void KinematicChain::setReferenceGeometry(const ReferenceGeometryPtr& rpGeometry)
{
  mpReferenceGeometry = rpGeometry;
}


std::vector<double> KinematicChain::getJointAngles() const
{
  std::vector<double> dummy(getNumberOfJoints());

  for(unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    dummy[i] = getJointAngle(i);
  }

  return dummy;
}


Mat KinematicChain::getJointAnglesMatrix() const
{
  Mat dummy = Mat::zeros(getNumberOfJoints(), 1, CV_64FC1);

  for(unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    dummy.at<double>(i,0) = getJointAngle(i);
  }

  return dummy;
}


double KinematicChain::getJointVelocity(unsigned int index) const
{
  if(index >= getNumberOfJoints())
  {
    return 0.0;
  }

  return mJointVelocities.at<double>(index, 0);
}


std::vector<double> KinematicChain::getJointVelocities() const
{
  std::vector<double> dummy(getNumberOfJoints());

  for(unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    dummy[i] = mJointVelocities.at<double>(i,0);
  }

  return dummy;
}


Mat KinematicChain::getJointVelocitiesMatrix() const
{
  return mJointVelocities.clone();
}


double KinematicChain::getJointAcceleration(unsigned int index) const
{
  if(index >= getNumberOfJoints())
  {
    return 0.0;
  }

  return mJointAccelerations.at<double>(index, 0);
}


std::vector<double> KinematicChain::getJointAccelerations() const
{
  std::vector<double> dummy(getNumberOfJoints());

  for(unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    dummy[i] = mJointAccelerations.at<double>(i,0);
  }

  return dummy;
}


cv::Mat KinematicChain::getJointAccelerationsMatrix() const
{
  return mJointAccelerations.clone();
}


void KinematicChain::setJointAngles(const std::vector<double>& angles)
{
  if(angles.size() != getNumberOfJoints())
  {
    cout << "Error: You provided an vector of angles with the wrong size ("
        << angles.size() << " != " << getNumberOfJoints() << ")!" << endl;
    return;
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double angle = angles[i];
    angle = max<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
    angle = min<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.max);

    setJointAngle(i, angle);
  }

  return;
}


void KinematicChain::setJointAngles(const Mat& angles)
{
  if(angles.size().height != (int)getNumberOfJoints() || angles.size().width != 1)
  {
    cout << "Error: You provided an matrix of angles with the wrong size [("
        << angles.size().height << "," << angles.size().width
        << ") != (" << getNumberOfJoints() << ",1)]!" << endl;
    return;
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double angle = angles.at<double>(i,0);
    angle = max<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
    angle = min<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.max);

    setJointAngle(i, angle);
  }

  return;
}


void KinematicChain::setJointVelocity(unsigned index, double velocity)
{
  if(mCurrentWorkingMode != VELOCITY)
  {
    return;
  }

  if(index >= getNumberOfJoints())
  {
    cout << "Error: Trying to set velocity for joint " << index << "!" << endl;
    return;
  }

  velocity = max<double>(velocity, mpReferenceGeometry->getJoint(index)->velocityLimits.min);
  velocity = min<double>(velocity, mpReferenceGeometry->getJoint(index)->velocityLimits.max);

  mJointVelocities.at<double>(index,0) = velocity;

  return;
}


void KinematicChain::setJointVelocities(const std::vector<double>& velocities)
{
  if(mCurrentWorkingMode != VELOCITY)
  {
    return;
  }

  if(velocities.size() != getNumberOfJoints())
  {
    cout << "Error: You provided an vector of velocities with the wrong size ("
        << velocities.size() << " != " << getNumberOfJoints() << ")!" << endl;
    return;
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double velocity = velocities[i];
    velocity = max<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.min);
    velocity = min<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.max);

    mJointVelocities.at<double>(i,0) = velocity;
  }

  return;
}


void KinematicChain::setJointVelocities(const cv::Mat& velocities)
{

  if(mCurrentWorkingMode != VELOCITY)
  {
    return;
  }

  if(velocities.size().height != (int)getNumberOfJoints() || velocities.size().width != 1)
  {
    cout << "Error: You provided an matrix of velocities with the wrong size [("
        << velocities.size().height << "," << velocities.size().width
        << ") != (" << getNumberOfJoints() << ",1)]!" << endl;
    return;
  }

  mJointVelocities = velocities;
  applyVelocityLimits(mJointVelocities);
  return;
}


void KinematicChain::setJointAcceleration(unsigned int index, double acceleration)
{
  if(mCurrentWorkingMode != ACCELERATION)
  {
    return;
  }

  if(index >= getNumberOfJoints())
  {
    cout << "Error: Trying to set acceleration for joint " << index << "!" << endl;
    return;
  }

  mJointAccelerations.at<double>(index,0) = acceleration;
  return;
}


void KinematicChain::setJointAccelerations(const std::vector<double>& accelerations)
{
  if(mCurrentWorkingMode != ACCELERATION)
  {
    return;
  }

  if(accelerations.size() != getNumberOfJoints())
  {
    cout << "Error: You provided an matrix of accelerations with the wrong size ("
        << accelerations.size() << " != " << getNumberOfJoints() << ")!" << endl;
    return;
  }

  for(unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    mJointAccelerations.at<double>(i,0) = accelerations[i];
  }

  return;
}


void KinematicChain::setJointAccelerations(const cv::Mat& accelerations)
{
  if(mCurrentWorkingMode != ACCELERATION)
  {
    return;
  }

  if(accelerations.size().height != (int)getNumberOfJoints() || accelerations.size().width != 1)
  {
    cout << "Error: You provided an matrix of accelerations with the wrong size [("
        << accelerations.size().height << "," << accelerations.size().width
        << ") != (" << getNumberOfJoints() << ",1)]!" << endl;
    return;
  }

  mJointAccelerations = accelerations;
  return;
}


void KinematicChain::step(double time)
{
  Mat angles, velocities;

  // update the angle according to working mode
  switch(mCurrentWorkingMode)
  {
    case ANGLE:

      break;

    case VELOCITY:

      angles = getJointAnglesMatrix();
      angles += getJointVelocitiesMatrix() * ( time / 1000.0 );
      applyAngleLimits(angles);

      setJointAngles(angles);
      break;

    case ACCELERATION:

      velocities = getJointVelocitiesMatrix();
      velocities += getJointAccelerationsMatrix() * ( time / 1000.0 );
      applyVelocityLimits(velocities);

      angles = getJointAnglesMatrix();
      angles += velocities * ( time / 1000.0 );
      applyAngleLimits(angles);

      mJointVelocities = velocities;
      setJointAngles(angles);
      break;

    default:
      cerr << "Oh oh, something went terribly wrong in cedar::dev::robot::KinematicChain::step(double)!" << endl;
  }

  return;
}


void KinematicChain::setWorkingMode(ActionType actionType)
{
  mCurrentWorkingMode = actionType;
  stop();
  init();
}


void KinematicChain::init()
{
  stop();
  mJointVelocities = Mat::zeros(getNumberOfJoints(), 1, CV_64FC1);
  mJointAccelerations = Mat::zeros(getNumberOfJoints(), 1, CV_64FC1);
  return;
}


void KinematicChain::applyAngleLimits(Mat &angles)
{

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double angle = angles.at<double>(i, 0);
    angle = max<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
    angle = min<double>(angle, mpReferenceGeometry->getJoint(i)->angleLimits.max);
    angles.at<double>(i, 0) = angle;
  }

  return;
}


void KinematicChain::applyVelocityLimits(Mat &velocities)
{

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double velocity = velocities.at<double>(i, 0);
    velocity = max<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.min);
    velocity = min<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.max);
    velocities.at<double>(i, 0) = velocity;
  }

  return;
}
