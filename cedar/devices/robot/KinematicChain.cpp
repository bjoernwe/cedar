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

//------------------------------------------------------------------------------
// constructors and destructor
//------------------------------------------------------------------------------

//! constructor
KinematicChain::KinematicChain(const cedar::dev::robot::ReferenceGeometryPtr& rpReferenceGeometry)
:
LoopedThread(50.5), //!\todo this step size should be set different, should be a parameter, i.e. read in from configuration file
mpReferenceGeometry(rpReferenceGeometry)
{
  mJointVelocities.resize(getNumberOfJoints());
  mJointAccelerations.resize(getNumberOfJoints());
  mJointWorkingModes.resize(getNumberOfJoints());
}

KinematicChain::KinematicChain(const std::string& configFileName)
:
LoopedThread(50.0, 0.001, configFileName), //!\todo this step size should be set different, should be a parameter, i.e. read in from configuration file
mpReferenceGeometry(new ReferenceGeometry(configFileName))
{
  mJointVelocities.resize(getNumberOfJoints());
  mJointAccelerations.resize(getNumberOfJoints());
  mJointWorkingModes.resize(getNumberOfJoints());
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


double KinematicChain::getJointVelocity(unsigned int index) const
{
  if(index >= getNumberOfJoints())
  {
    return 0.0;
  }
  return mJointVelocities[index];
}


std::vector<double> KinematicChain::getJointVelocities() const
{
  std::vector<double> dummy(mJointVelocities);
  return dummy;
}


cv::Mat KinematicChain::getJointVelocitiesMatrix() const
{
  cv::Mat dummy(getNumberOfJoints(), 1, CV_64FC1);
  //!\todo check matrix type
  for (unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    dummy.at<double>(i,0) = mJointVelocities[i];
  }
  return dummy;
}


double KinematicChain::getJointAcceleration(unsigned int index) const
{
  if(index >= getNumberOfJoints())
  {
    return 0.0;
  }
  return mJointAccelerations[index];
}


std::vector<double> KinematicChain::getJointAccelerations() const
{
  std::vector<double> dummy(mJointAccelerations);
  return dummy;
}


cv::Mat KinematicChain::getJointAccelerationsMatrix() const
{
  cv::Mat dummy(getNumberOfJoints(), 1, CV_64FC1);
  for (unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    dummy.at<double>(i,0) = mJointAccelerations[i];
  }
  return dummy;
}

void KinematicChain::setJointVelocity(unsigned index, double velocity)
{
  if(index >= getNumberOfJoints())
  {
    return;
  }

  velocity = max<double>(velocity, mpReferenceGeometry->getJoint(index)->velocityLimits.min);
  velocity = min<double>(velocity, mpReferenceGeometry->getJoint(index)->velocityLimits.max);

  mJointVelocities[index] = velocity;
  mJointWorkingModes[index] = VELOCITY;

  return;
}


void KinematicChain::setJointVelocities(const std::vector<double>& velocities)
{
  if(velocities.size() != getNumberOfJoints())
  {
    return;
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double velocity = velocities[i];
    velocity = max<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.min);
    velocity = min<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.max);

    mJointVelocities[i] = velocity;
    mJointWorkingModes[i] = VELOCITY;
  }

  return;
}


void KinematicChain::setJointVelocities(const cv::Mat& velocities)
{

  if(velocities.size().height != (int)getNumberOfJoints() || velocities.size().width != 1)
  {
    return;
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    double velocity = velocities.at<double>(i,0);

    velocity = max<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.min);
    velocity = min<double>(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.max);

    mJointVelocities[i] = velocity;
    mJointWorkingModes[i] = VELOCITY;
  }

  return;
}


void KinematicChain::setJointAcceleration(unsigned int index, double acceleration)
{
  if(index >= getNumberOfJoints())
  {
    return;
  }
  mJointAccelerations[index] = acceleration;
  mJointWorkingModes[index] = ACCELERATION;
}


void KinematicChain::setJointAccelerations(const std::vector<double>& accelerations)
{
  if(accelerations.size() != getNumberOfJoints())
  {
    return;
  }
  mJointAccelerations = accelerations;
  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    mJointWorkingModes[i] = ACCELERATION;
  }
}


void KinematicChain::setJointAccelerations(const cv::Mat& accelerations)
{
  if(accelerations.size().height != (int)getNumberOfJoints() || accelerations.size().width != 1)
  {
    return;
  }
  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    mJointAccelerations[i] = accelerations.at<double>(i,0);
    mJointWorkingModes[i] = ACCELERATION;
  }
}


void KinematicChain::step(double time)
{

//  cout << "running KinematicChain::step(" << time << ")" << endl;

  double currentAngle = 0.0;
  double newAngle = 0.0;
  double velocity = 0.0;

  // update joint angle depending on working mode
  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    // get current joint angle from the device
    currentAngle = getJointAngle(i);
//    cout << "currentAngle = " << currentAngle << endl;

    // update the angle according to working mode
    switch(mJointWorkingModes[i])
    {

    case ANGLE:
      // calculate velocity
      newAngle = getJointAngles()[i];
//      cout << "newAngle = " << newAngle << endl;
      velocity = (newAngle - currentAngle) * (1000.0 / time);
//      cout << "velocity = " << velocity << endl;

      // consider limits
      newAngle = max<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
      newAngle = min<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.max);
//      cout << "newAngle* = " << newAngle << endl;

      // apply new values
      setJointAngle(i, newAngle);
      //!\todo setJointAngle is called for each joint separately. Change this to only call it once for a vector of angles
      mJointVelocities[i] = velocity;

      break;

    case VELOCITY:
//      cout << "velocity mode" << endl;

      // calculate new angle
      velocity = mJointVelocities[i];
      newAngle = currentAngle + velocity * (time / 1000.0);
//      cout << "newAngle = " << newAngle << endl;

      // consider angle limits
      newAngle = max<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
      newAngle = min<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.max);
//      cout << "newAngle = " << newAngle << endl;

      // set new joint angle
      setJointAngle(i, newAngle);

      break;

    case ACCELERATION:

      velocity = mJointVelocities[i] + mJointAccelerations[i] * (time / 1000.0);

      // consider velocity limits
      velocity = max(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.min);
      velocity = min(velocity, mpReferenceGeometry->getJoint(i)->velocityLimits.max);

      newAngle = currentAngle + velocity * (time / 1000.0);

      // consider angle limits
      newAngle = max<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.min);
      newAngle = min<double>(newAngle, mpReferenceGeometry->getJoint(i)->angleLimits.max);

      // set new values
      setJointAngle(i, newAngle);
      mJointVelocities[i] = velocity;

      break;

    default:
      cerr << "Oh oh, something went terribly wrong in cedar::dev::robot::KinematicChain::step(double time)!" << endl;
    }
  }
  return;
}
