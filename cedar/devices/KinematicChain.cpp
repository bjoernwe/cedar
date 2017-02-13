/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014, 2015, 2016, 2017 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

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

    Maintainer:  Jean-Stephane Jokeit   
    Email:       jean-stephane.jokeit@ini.rub.de
    Date:        2010 11 15

    Description: Chain of joints (e.g., a robotic arm).

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/devices/KinematicChain.h"
#include "cedar/devices/exceptions.h"
#include "cedar/devices/ForwardKinematics.h"
#include "cedar/auxiliaries/LocalCoordinateFrame.h"


#include "cedar/auxiliaries/math/LimitsParameter.h"
#include "cedar/auxiliaries/Singleton.h"
#include "cedar/auxiliaries/FactoryManager.h"
#include "cedar/auxiliaries/math/constants.h"
#include "cedar/auxiliaries/math/screwCalculus.h"
#include "cedar/units/Length.h"
#include "cedar/auxiliaries/exceptions.h" // for DuplicateNameException

// SYSTEM INCLUDES
#ifndef Q_MOC_RUN
  #include <boost/lexical_cast.hpp>
#endif
#include <algorithm>

namespace
{
  bool registered()
  {
    cedar::dev::JointFactoryManagerSingleton::getInstance()->registerType<cedar::dev::KinematicChain::JointPtr>();

    cedar::dev::JointFactoryManagerSingleton::getInstance()->addDeprecatedName<cedar::dev::KinematicChain::JointPtr>("cedar.dev.robot.KinematicChain.Joint");

    return true;
  }

  bool registeredJoint = registered();
}

//------------------------------------------------------------------------------
// static variables
//------------------------------------------------------------------------------

const cedar::dev::Component::ComponentDataType cedar::dev::KinematicChain::JOINT_ANGLES = 1;
const cedar::dev::Component::ComponentDataType cedar::dev::KinematicChain::JOINT_VELOCITIES = 2;
const cedar::dev::Component::ComponentDataType cedar::dev::KinematicChain::JOINT_ACCELERATIONS = 3;
const cedar::dev::Component::ComponentDataType cedar::dev::KinematicChain::JOINT_TORQUES = 4;

//------------------------------------------------------------------------------
// constructors and destructor
//------------------------------------------------------------------------------

//! constructor
cedar::dev::KinematicChain::KinematicChain
(
  cedar::aux::LocalCoordinateFramePtr pEndEffector
)
:
mpJoints(new JointListParameter(this, "joints")),
mForwardKinematics(new cedar::dev::ForwardKinematics(this, pEndEffector))
{
  init();
}

//! constructor
cedar::dev::KinematicChain::Joint::Joint()
:
_mpPosition(new cedar::aux::DoubleVectorParameter(this, "position", cedar::aux::DoubleVectorParameter::LimitType(-10.0, 10.0))),
_mpAxis(new cedar::aux::DoubleVectorParameter(this, "axis", cedar::aux::DoubleVectorParameter::LimitType(-1.0, 1.0))),
_mpAngleLimits
(
  new cedar::aux::math::LimitsParameter<double>
  (
    this,
    "angle limits",
    -2 * cedar::aux::math::pi,
    -2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi
  )
),
_mpVelocityLimits
(
  new cedar::aux::math::LimitsParameter<double>
  (
    this,
    "velocity limits",
    -2 * cedar::aux::math::pi,
    -2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi
  )
),
_mpAccelerationLimits
(
  new cedar::aux::math::LimitsParameter<double>
  (
    this,
    "acceleration limits",
    -2 * cedar::aux::math::pi,
    -2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi,
    0.0,
    2 * cedar::aux::math::pi
  )
)
{

}

cedar::dev::KinematicChain::~KinematicChain()
{
  prepareComponentDestructAbsolutelyRequired();
}

//------------------------------------------------------------------------------
// methods
//------------------------------------------------------------------------------
void cedar::dev::KinematicChain::updateTransformations()
{
//  calculateTransformations();
}

void cedar::dev::KinematicChain::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  cedar::aux::NamedConfigurable::readConfiguration(node);
  initializeFromJointList();
  getRootCoordinateFrame()->setName(this->getName());
}

unsigned int cedar::dev::KinematicChain::getNumberOfJoints() const
{
  return mpJoints->size();
}

float cedar::dev::KinematicChain::getJointAngle(unsigned int index) const
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }

  //!@todo doesn't this function also check for violations of joint range?
  return getUserSideMeasurementBufferIndex(JOINT_ANGLES, index);
}

cv::Mat cedar::dev::KinematicChain::getJointAngles() const
{
  return getUserSideMeasurementBuffer(JOINT_ANGLES);
}

cv::Mat cedar::dev::KinematicChain::getCachedJointAngles() const
{
  return getJointAngles();
}

cv::Mat cedar::dev::KinematicChain::getJointAnglesMatrix() const
{
  return getJointAngles();
}

float cedar::dev::KinematicChain::getJointVelocity(unsigned int index) const
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }
  //!@todo doesn't this function also check for violations of joint range?
  return getUserSideMeasurementBufferIndex( JOINT_VELOCITIES, index );
}

cv::Mat cedar::dev::KinematicChain::getJointVelocities() const
{
  return getUserSideMeasurementBuffer( JOINT_VELOCITIES );
}

cv::Mat cedar::dev::KinematicChain::getCachedJointVelocities() const
{
  return getJointVelocities();
}


cv::Mat cedar::dev::KinematicChain::getJointVelocitiesMatrix() const
{
  return getJointVelocities();
}

float cedar::dev::KinematicChain::getJointAcceleration(unsigned int index) const
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }
  //!@todo doesn't this function also check for violations of joint range?
  return getUserSideMeasurementBufferIndex(JOINT_ACCELERATIONS, index);
}

cv::Mat cedar::dev::KinematicChain::getJointAccelerations() const
{
  return getUserSideMeasurementBuffer(JOINT_ACCELERATIONS);
}

cv::Mat cedar::dev::KinematicChain::getCachedJointAccelerations() const
{
  return getJointAccelerations();
}

cv::Mat cedar::dev::KinematicChain::getJointAccelerationsMatrix() const
{
  return getJointAccelerations();
}

void cedar::dev::KinematicChain::setJointAngle(unsigned int index, float value)
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }
  //!@todo use applyVelocityLimits()?
  value = std::max<float>(value, getJoint(index)->_mpAngleLimits->getLowerLimit());
  value= std::min<float>(value, getJoint(index)->_mpAngleLimits->getUpperLimit());
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBufferIndex(JOINT_ANGLES, index, value);
}


void cedar::dev::KinematicChain::setJointAngles(const std::vector<float>& angles)
{
  //!@todo: for security reasons setting angles should be only allowed in STOP or ANGLE mode. except initial set!

  if (angles.size() != getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a vector of angles with the wrong size (angles: "
              + cedar::aux::toString(angles.size())
              + " != no. of joints: "
              + cedar::aux::toString(getNumberOfJoints())
              + ")!"
    );
  }
  //!@todo put this in a matrix and use setJointAngles(matrix)
  for (unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    //!@todo doesn't this function also check for violations of joint range?
    setJointAngle(i, angles[i]);
  }
}

void cedar::dev::KinematicChain::setJointAngles(const cv::Mat& angles)
{
  if (angles.size().height != (int)getNumberOfJoints() || angles.size().width != 1)
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a matrix of angles with the wrong size (angles: "
        + cedar::aux::toString(angles.rows) + "x" + cedar::aux::toString(angles.cols)
        + " != expected: "
        + cedar::aux::toString(getNumberOfJoints())
        + "x1)!"
    );
  }

  cv::Mat new_angles = angles.clone();

  for (unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    //!@todo: use applyAngleLimits() ?
    float angle = angles.at<float>(i,0);
    angle = std::max<float>(angle, getJoint(i)->_mpAngleLimits->getLowerLimit());
    angle = std::min<float>(angle, getJoint(i)->_mpAngleLimits->getUpperLimit());

    new_angles.at<float>(i,0) = angle;
  }
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBuffer(JOINT_ANGLES, new_angles);
}

void cedar::dev::KinematicChain::setJointVelocity(unsigned int index, float velocity)
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }
  //!@todo use applyVelocityLimits()?
  velocity = std::max<float>(velocity, getJoint(index)->_mpVelocityLimits->getLowerLimit());
  velocity = std::min<float>(velocity, getJoint(index)->_mpVelocityLimits->getUpperLimit());
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBufferIndex(JOINT_VELOCITIES, index, velocity);
}

void cedar::dev::KinematicChain::setJointVelocities(const std::vector<float>& velocities)
{
  if (velocities.size() != getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a vector of velocities with the wrong size (velocities: "
              + cedar::aux::toString(velocities.size())
              + " != no. of joints: "
              + cedar::aux::toString(getNumberOfJoints())
              + ")!"
    );
  }

  for(unsigned i = 0; i < getNumberOfJoints(); i++)
  {
    // locking done in setJointVelocity()
    //!@todo doesn't this function also check for violations of joint range?
    setJointVelocity(i, velocities[i]);
  }
}

void cedar::dev::KinematicChain::setJointVelocities(const cv::Mat& velocities)
{
  if (velocities.size().height != (int)getNumberOfJoints() || velocities.size().width != 1)
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a matrix of velocities with the wrong size (velocities: "
        + cedar::aux::toString(velocities.rows) + "x" + cedar::aux::toString(velocities.cols)
        + " != expected: "
        + cedar::aux::toString(getNumberOfJoints())
        + "x1)!"
    );
  }

  cv::Mat new_vels = velocities.clone();

  // TODO: test limits
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBuffer( JOINT_VELOCITIES, new_vels );
}

void cedar::dev::KinematicChain::setJointAcceleration(unsigned int index, float acceleration)
{
  if (index >= this->getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointIndexOutOfRangeException,
      "Joint index " + cedar::aux::toString(index)
      + " exceeds number of joints, which is " + cedar::aux::toString(this->getNumberOfJoints()) + "."
    );
  }
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBufferIndex(JOINT_ACCELERATIONS, index, acceleration);
}

void cedar::dev::KinematicChain::setJointAccelerations(const std::vector<float>& accelerations)
{
  if (accelerations.size() != getNumberOfJoints())
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a vector of accelerations with the wrong size (accelerations: "
              + cedar::aux::toString(accelerations.size())
              + " != no. of joints: "
              + cedar::aux::toString(getNumberOfJoints())
              + ")!"
    );
  }

  for (unsigned int i = 0; i < getNumberOfJoints(); ++i)
  {
    //!@todo doesn't this function also check for violations of joint range?
    setJointAcceleration(i, accelerations[i]);
  }
}

void cedar::dev::KinematicChain::setJointAccelerations(const cv::Mat& accelerations)
{
  if (accelerations.size().height != (int)getNumberOfJoints() || accelerations.size().width != 1)
  {
    CEDAR_THROW
    (
      cedar::dev::JointNumberMismatchException,
      "You provided a matrix of accelerations with the wrong size (accelerations: "
        + cedar::aux::toString(accelerations.rows) + "x" + cedar::aux::toString(accelerations.cols)
        + " != expected: "
        + cedar::aux::toString(getNumberOfJoints())
        + "x1)!"
    );
  }

  cv::Mat new_accels = accelerations.clone();
  // todo: test limits
  //!@todo doesn't this function also check for violations of joint range?
  setUserSideCommandBuffer(JOINT_ACCELERATIONS, new_accels);
}

// TODO: applyJointLimits, applyJointVelocityLimits, etc

void cedar::dev::KinematicChain::init()
{
  connect
  (
    this,
    SIGNAL(updatedUserMeasurementSignal()),
    this,
    SLOT(updatedUserMeasurementSlot()), Qt::DirectConnection
  );

  installCommandAndMeasurementType(cedar::dev::KinematicChain::JOINT_ANGLES, "Joint Angles");
  installCommandAndMeasurementType(cedar::dev::KinematicChain::JOINT_VELOCITIES, "Joint Velocities");
  installCommandAndMeasurementType(cedar::dev::KinematicChain::JOINT_ACCELERATIONS, "Joint Accelerations");
//  installCommandAndMeasurementType(cedar::dev::KinematicChain::JOINT_TORQUES, "Joint Torques");

  registerCommandTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
    cedar::dev::KinematicChain::JOINT_VELOCITIES,
    boost::bind(&cedar::dev::Component::integrateDevice, this, _1, _2, cedar::dev::KinematicChain::JOINT_VELOCITIES)
  );
  registerCommandTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
    cedar::dev::KinematicChain::JOINT_ANGLES,
    boost::bind
    (
      &cedar::dev::Component::integrateDeviceTwice,
      this,
      _1,
      _2,
      cedar::dev::KinematicChain::JOINT_VELOCITIES,
      cedar::dev::KinematicChain::JOINT_ANGLES
    )
  );
  registerCommandTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_VELOCITIES,
    cedar::dev::KinematicChain::JOINT_ANGLES,
    boost::bind(&cedar::dev::Component::integrateDevice, this, _1, _2, cedar::dev::KinematicChain::JOINT_ANGLES)
  );

  registerMeasurementTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
    cedar::dev::KinematicChain::JOINT_VELOCITIES,
    boost::bind(&cedar::dev::Component::integrateDevice, this, _1, _2, cedar::dev::KinematicChain::JOINT_VELOCITIES)
  );
  registerMeasurementTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
    cedar::dev::KinematicChain::JOINT_ANGLES,
    boost::bind
    (
      &cedar::dev::Component::integrateDeviceTwice,
      this,
      _1,
      _2,
      cedar::dev::KinematicChain::JOINT_ANGLES,
      cedar::dev::KinematicChain::JOINT_VELOCITIES
    )
  );
  registerMeasurementTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_VELOCITIES,
    cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
    boost::bind(&cedar::dev::Component::differentiateDevice, this, _1, _2, cedar::dev::KinematicChain::JOINT_VELOCITIES)
  );
  registerMeasurementTransformationHook
  (
    cedar::dev::KinematicChain::JOINT_ANGLES,
    cedar::dev::KinematicChain::JOINT_VELOCITIES,
    boost::bind(&cedar::dev::Component::differentiateDevice, this, _1, _2, cedar::dev::KinematicChain::JOINT_ANGLES)
  );
  registerMeasurementTransformationHook
  (
      cedar::dev::KinematicChain::JOINT_ANGLES,
      cedar::dev::KinematicChain::JOINT_ACCELERATIONS,
      boost::bind
      (
        &cedar::dev::Component::differentiateDeviceTwice,
        this,
        _1,
        _2,
        cedar::dev::KinematicChain::JOINT_ANGLES,
        cedar::dev::KinematicChain::JOINT_VELOCITIES
      )
  );

  registerCheckCommandHook
  (
    boost::bind
    (
      &cedar::dev::KinematicChain::applyLimits,
      this,
      _1,
      _2
    )
  );

  this->addConfigurableChild("root coordinate frame", mForwardKinematics->getRootCoordinateFrame());
  this->addConfigurableChild("end-effector coordinate frame", mForwardKinematics->getEndEffectorCoordinateFrame());

  cedar::dev::KinematicChain::JointPtr default_joint(new cedar::dev::KinematicChain::Joint);
  default_joint->_mpPosition->pushBack(0);
  default_joint->_mpPosition->pushBack(0);
  default_joint->_mpPosition->pushBack(0);
  default_joint->_mpAxis->pushBack(0);
  default_joint->_mpAxis->pushBack(0);
  default_joint->_mpAxis->pushBack(1);
  default_joint->_mpAngleLimits->setLowerLimit(-2 * cedar::aux::math::pi);
  default_joint->_mpAngleLimits->setUpperLimit(2 * cedar::aux::math::pi);
  default_joint->_mpVelocityLimits->setLowerLimit(-2 * cedar::aux::math::pi);
  default_joint->_mpVelocityLimits->setUpperLimit(2 * cedar::aux::math::pi);
  default_joint->_mpAccelerationLimits->setLowerLimit(-2 * cedar::aux::math::pi);
  default_joint->_mpAccelerationLimits->setUpperLimit(2 * cedar::aux::math::pi);
  this->mpJoints->pushBack(default_joint);
  initializeFromJointList();

  checkInitialConfigurations();
}

bool cedar::dev::KinematicChain::applyLimits(const cedar::dev::Component::ComponentDataType &type, cv::Mat &data)
{
  switch(type)
  {
    case cedar::dev::KinematicChain::JOINT_ANGLES:
      applyAngleLimits(data);
      break;

    case cedar::dev::KinematicChain::JOINT_VELOCITIES:
      applyVelocityLimits(data);
      break;

    case cedar::dev::KinematicChain::JOINT_ACCELERATIONS:
      applyAccelerationLimits(data);
      break;

    default:
      cedar::aux::LogSingleton::getInstance()->warning(
         "Component data type " + cedar::aux::toString(type) + " is not known.",
          CEDAR_CURRENT_FUNCTION_NAME);
      return false;
  }

  return true;
}

void cedar::dev::KinematicChain::initializeFromJointList()
{
  mForwardKinematics->initializeFromJointList();

  auto num = getNumberOfJoints();

  setCommandAndMeasurementDimensionality( cedar::dev::KinematicChain::JOINT_ANGLES, num );
  setCommandAndMeasurementDimensionality( cedar::dev::KinematicChain::JOINT_VELOCITIES, num );
  setCommandAndMeasurementDimensionality( cedar::dev::KinematicChain::JOINT_ACCELERATIONS, num );
  //setCommandAndMeasurementDimensionality( cedar::dev::KinematicChain::JOINT_TORQUES, num );

  // warning vector shall contain warning counters, each for every different kind of limit excess (angle, velocity, acceleration)
  mWarned.resize(num * 3);
}

void cedar::dev::KinematicChain::applyAngleLimits(cv::Mat& angles)
{
  const unsigned int num_joints = getNumberOfJoints();

  for (unsigned i = 0; i < num_joints; i++)
  {
    float angle = angles.at<float>(i, 0);
    const float old_angle = angle;

    const float &lower_limit = getJoint(i)->_mpAngleLimits->getLowerLimit();
    const float &upper_limit = getJoint(i)->_mpAngleLimits->getUpperLimit();

    angle = std::max<float>(angle, lower_limit);

    if(angle == lower_limit)
    {            
      if(mWarned[i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": angle " + cedar::aux::toString(old_angle) + " has been capped at " + cedar::aux::toString(lower_limit),
          CEDAR_CURRENT_FUNCTION_NAME);

        mWarned[i] = 0;
      }

      ++mWarned[i];
    }

    angle = std::min<float>(angle, upper_limit);

    if(angle == upper_limit)
    {
      if(mWarned[i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": angle " + cedar::aux::toString(old_angle) + " has been capped at " + cedar::aux::toString(upper_limit),
          CEDAR_CURRENT_FUNCTION_NAME);

        mWarned[i] = 0;
      }

      ++mWarned[i];
    }

    angles.at<float>(i, 0) = angle;
  }
}


void cedar::dev::KinematicChain::applyVelocityLimits(cv::Mat& velocities)
{
  const unsigned int num_joints = getNumberOfJoints();

  for (unsigned i = 0; i < num_joints; i++)
  {
    float velocity = velocities.at<float>(i, 0);
    const float old_velocity = velocity;

    const float &lower_limit = getJoint(i)->_mpVelocityLimits->getLowerLimit();
    const float &upper_limit = getJoint(i)->_mpVelocityLimits->getUpperLimit();

    velocity = std::max<float>(velocity, lower_limit);

    if(velocity == lower_limit)
    {
      if(mWarned[num_joints+i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": velocity " + cedar::aux::toString(old_velocity) + " has been capped at " + cedar::aux::toString(lower_limit),
          CEDAR_CURRENT_FUNCTION_NAME);

        mWarned[num_joints+i] = 0;
      }

      ++mWarned[num_joints+i];
    }

    velocity = std::min<float>(velocity, upper_limit);

    if(velocity == upper_limit)
    {
      if(mWarned[num_joints+i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": velocity " + cedar::aux::toString(old_velocity) + " has been capped at " + cedar::aux::toString(upper_limit),
          CEDAR_CURRENT_FUNCTION_NAME);
        mWarned[num_joints+i] = 0;
      }

      ++mWarned[num_joints+i];
    }

    velocities.at<float>(i, 0) = velocity;
  }
}

void cedar::dev::KinematicChain::applyAccelerationLimits(cv::Mat& accelerations)
{
  const unsigned int num_joints = getNumberOfJoints();

  for (unsigned i = 0; i < num_joints; i++)
  {
    float acceleration = accelerations.at<float>(i, 0);
    const float old_acceleration = acceleration;

    const float &lower_limit = getJoint(i)->_mpVelocityLimits->getLowerLimit();
    const float &upper_limit = getJoint(i)->_mpVelocityLimits->getUpperLimit();

    acceleration = std::max<float>(acceleration, lower_limit);

    if(acceleration == lower_limit)
    {
      if(mWarned[2*num_joints + i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": acceleration " + cedar::aux::toString(old_acceleration) + " has been capped at " + cedar::aux::toString(lower_limit),
          CEDAR_CURRENT_FUNCTION_NAME);
        mWarned[2*num_joints + i] = 0;
      }

      ++mWarned[2*num_joints + i];
    }

    acceleration = std::min<float>(acceleration, upper_limit);

    if(acceleration == upper_limit)
    {
      if(mWarned[2*num_joints + i] % 1000 == 0)
      {
        cedar::aux::LogSingleton::getInstance()->warning(
          this->prettifyName()+", joint " + cedar::aux::toString(i) + ": acceleration " + cedar::aux::toString(old_acceleration) + " has been capped at " + cedar::aux::toString(upper_limit),
          CEDAR_CURRENT_FUNCTION_NAME);
        mWarned[2*num_joints + i] = 0;
      }

      ++mWarned[2*num_joints + i];
    }

    accelerations.at<float>(i, 0) = acceleration;
  }
}

cedar::aux::LocalCoordinateFramePtr cedar::dev::KinematicChain::getEndEffectorCoordinateFrame()
{
  return mForwardKinematics->getEndEffectorCoordinateFrame();
}

cedar::aux::LocalCoordinateFramePtr cedar::dev::KinematicChain::getRootCoordinateFrame()
{
  return mForwardKinematics->getRootCoordinateFrame();
}

cv::Mat cedar::dev::KinematicChain::getRootTransformation()
{
  return mForwardKinematics->getRootTransformation();
}

void cedar::dev::KinematicChain::setEndEffector(cedar::aux::LocalCoordinateFramePtr pEndEffector)
{
  mForwardKinematics->setEndEffector(pEndEffector);
}

cv::Mat cedar::dev::KinematicChain::getJointTransformation(unsigned int index)
{
  return mForwardKinematics->getJointTransformation(index);
}

cv::Mat cedar::dev::KinematicChain::calculateEndEffectorPosition()
{
  return mForwardKinematics->calculateEndEffectorPosition();
}

void cedar::dev::KinematicChain::calculateCartesianJacobian
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    cv::Mat& result,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateCartesianJacobian(point, jointIndex, result, coordinateFrame);
}


cv::Mat cedar::dev::KinematicChain::calculateCartesianJacobian
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateCartesianJacobian(point, jointIndex, coordinateFrame);
}

void cedar::dev::KinematicChain::calculateCartesianJacobianTemporalDerivative
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    cv::Mat& result,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateCartesianJacobianTemporalDerivative(point, jointIndex, result, coordinateFrame);
}


cv::Mat cedar::dev::KinematicChain::calculateCartesianJacobianTemporalDerivative
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateCartesianJacobianTemporalDerivative(point, jointIndex, coordinateFrame);
}

cv::Mat cedar::dev::KinematicChain::calculateVelocity
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateVelocity(point, jointIndex, coordinateFrame);
}

cv::Mat cedar::dev::KinematicChain::calculateSpatialJacobian(unsigned int index)
{
  return mForwardKinematics->calculateSpatialJacobian(index);
}

cv::Mat cedar::dev::KinematicChain::calculateSpatialJacobianTemporalDerivative(unsigned int index)
{
  return mForwardKinematics->calculateSpatialJacobianTemporalDerivative(index);
}

cv::Mat cedar::dev::KinematicChain::calculateAcceleration
  (
    const cv::Mat& point,
    unsigned int jointIndex,
    unsigned int coordinateFrame
  )
{
  return mForwardKinematics->calculateAcceleration(point, jointIndex, coordinateFrame);
}

cv::Mat cedar::dev::KinematicChain::getProductOfExponentials(unsigned int jointIndex)
{
  return mForwardKinematics->getProductOfExponentials(jointIndex);
}

cv::Mat cedar::dev::KinematicChain::getEndEffectorTransformation()
{
  return mForwardKinematics->getEndEffectorTransformation();
}

cv::Mat cedar::dev::KinematicChain::calculateEndEffectorJacobian()
{
  return mForwardKinematics->calculateEndEffectorJacobian();
}

cv::Mat cedar::dev::KinematicChain::calculateEndEffectorVelocity()
{
  return mForwardKinematics->calculateEndEffectorVelocity();
}

cv::Mat cedar::dev::KinematicChain::calculateEndEffectorAcceleration()
{
  return mForwardKinematics->calculateEndEffectorAcceleration();
}

const cedar::dev::KinematicChain::JointPtr cedar::dev::KinematicChain::getJoint
(
  unsigned int index
) const
{
  return mpJoints->at(index);
}

void cedar::dev::KinematicChain::setInitialConfigurations(std::map<std::string, cv::Mat> configs)
{
  QWriteLocker wlock(&mCurrentInitialConfigurationLock);

  //mInitialConfigurations.clear();
  for (auto it = configs.begin(); it != configs.end(); ++it)
  {
    mInitialConfigurations[it->first] = it->second.clone();
    // it is important to clone the cv::Mats!
  }

  wlock.unlock();
  checkInitialConfigurations();
}

void cedar::dev::KinematicChain::addInitialConfiguration(const std::string &name, const cv::Mat &config)
{
  QWriteLocker wlock(&mCurrentInitialConfigurationLock);

  auto found = mInitialConfigurations.find(name);

  // if it already exists, simply override
  if (found != mInitialConfigurations.end())
  {
    mInitialConfigurations.erase(found);
  }

  mInitialConfigurations[ name ] = config.clone();

  wlock.unlock();
  checkInitialConfigurations();
}

void cedar::dev::KinematicChain::deleteInitialConfiguration(const std::string &name)
{
  QWriteLocker wlock(&mCurrentInitialConfigurationLock);

  auto found = mInitialConfigurations.find(name);
  if (found == mInitialConfigurations.end())
  {
    CEDAR_THROW
    (
      InitialConfigurationNotFoundException,
      "You are deleting the initial configuration " + name + " which doesn't exist."
    );
  }

  mInitialConfigurations.erase(found);

  wlock.unlock();
  checkInitialConfigurations();
}

bool cedar::dev::KinematicChain::hasInitialConfiguration(const std::string& name)
{
  QWriteLocker wlock(&mCurrentInitialConfigurationLock);

  auto found = mInitialConfigurations.find(name);
  if (found != mInitialConfigurations.end())
  {
    return true;
  }
  return false;
}

void cedar::dev::KinematicChain::checkInitialConfigurations()
{
  QReadLocker rlock(&mCurrentInitialConfigurationLock);
  
  // no known initial configuration ...
  if (mInitialConfigurations.empty())
  {
    rlock.unlock();
    // ... set a default initial configuration:
    addInitialConfiguration("zeros", cv::Mat::zeros(getNumberOfJoints(), 1, CV_32FC1) );
    // note, this recurses back to checkInitialConfigurations() but not into this if-branch
    return;
  }

  // non-empty initial configuration map:
  if (mCurrentInitialConfiguration.empty()
      || mCurrentInitialConfiguration == "")
  {
    rlock.unlock();
    // does not apply, only sets the currently active initial config:
    setCurrentInitialConfiguration( mInitialConfigurations.begin()->first );
  }
}

bool cedar::dev::KinematicChain::setCurrentInitialConfiguration(const std::string &s)
{
  // intentionally no checks here
  // private method (maybe this doesnt need to be private?)
  // we are sure that s is a valid configuration

  QWriteLocker wlock(&mCurrentInitialConfigurationLock);

  mCurrentInitialConfiguration = s;
  return true; //TODO: should this use a return value to communicate potential errors?
}

void cedar::dev::KinematicChain::applyInitialConfiguration(const std::string& name)
{
  QReadLocker rlock(&mCurrentInitialConfigurationLock);

  auto f = mInitialConfigurations.find(name);

  if (f != mInitialConfigurations.end())
  {
    rlock.unlock();

    setCurrentInitialConfiguration(name);
    //std::cout << f->second;
    setInitialUserSideCommandBuffer(cedar::dev::KinematicChain::JOINT_ANGLES, f->second);

    return;
  }
  else
  {
    CEDAR_THROW
    (
      InitialConfigurationNotFoundException,
      "You tried to apply an initial configuration that was not registered."
    );
  }

}

void cedar::dev::KinematicChain::applyInitialConfiguration(unsigned int index)
{
  QReadLocker rlock(&mCurrentInitialConfigurationLock);

  if (index >= mInitialConfigurations.size())
  {
    CEDAR_THROW
    (
      InitialConfigurationNotFoundException,
      "You tried to apply an initial configuration with index "
                       + boost::lexical_cast<std::string>(index)
                       + "' which doesn't exist. Size: "
                       + boost::lexical_cast<std::string>(mInitialConfigurations.size())
    );
  }

  unsigned int j = 0;
  for (auto it = mInitialConfigurations.begin(); it != mInitialConfigurations.end(); it++ )
  {
    if (index == j)
    {
      rlock.unlock();
      return applyInitialConfiguration(it->first);
    }
    j++;
  }
}

unsigned int cedar::dev::KinematicChain::getCurrentInitialConfigurationIndex()
{
  QReadLocker lock(&mCurrentInitialConfigurationLock);
  unsigned int j = 0;

  for (auto it = mInitialConfigurations.begin(); it != mInitialConfigurations.end(); it++)
  {
    if (it->first == mCurrentInitialConfiguration)
    {
      return j;
    }
    j++;
  }

  CEDAR_THROW( cedar::aux::InvalidNameException,
               "Current initial configuration index is not a valid "
               "initial configuration" );
  return 0;
}

cv::Mat cedar::dev::KinematicChain::getInitialConfiguration(const std::string& name)
{
  QReadLocker lock(&mCurrentInitialConfigurationLock);

  auto f = mInitialConfigurations.find(name);
  if (f == mInitialConfigurations.end())
  {
    CEDAR_THROW( cedar::aux::InvalidNameException,
                 "You requested initial configuration "
                 + name + " which is not registered. "
                 "Use addInitialConfiguration().");
  }

  return f->second;
}

std::vector<std::string> cedar::dev::KinematicChain::getInitialConfigurationIndices()
{
  QReadLocker lock(&mCurrentInitialConfigurationLock);
  std::vector<std::string> result;

  for (auto it = mInitialConfigurations.begin(); it != mInitialConfigurations.end(); ++it)
  {
    result.push_back( (*it).first );
  }
  return result;
}

cv::Mat cedar::dev::KinematicChain::getCurrentInitialConfiguration()
{
  QReadLocker lock(&mCurrentInitialConfigurationLock);

  std::map< std::string, cv::Mat >::const_iterator found = mInitialConfigurations.find(mCurrentInitialConfiguration);
  if (found == mInitialConfigurations.end())
  {
    // you cant really land here, but lets be paranoid:
    CEDAR_THROW(cedar::aux::InvalidNameException,
                "You requested the current initial configuration, "
                "but none is set. "
                "Use addInitialConfiguration().");
  }

  return mInitialConfigurations[mCurrentInitialConfiguration];
}

void cedar::dev::KinematicChain::updatedUserMeasurementSlot()
{
  mForwardKinematics->calculateTransformations();
}

bool cedar::dev::KinematicChain::applyBrakeSlowlyController()
{
  setController( cedar::dev::KinematicChain::JOINT_VELOCITIES,
                 boost::bind< cv::Mat>( [&]()
                              {
                                return getJointVelocities() / 4;
                              } 
                            ) );
  return true;
}

bool cedar::dev::KinematicChain::applyBrakeNowController()
{
  setController( cedar::dev::KinematicChain::JOINT_VELOCITIES,
                 boost::bind< cv::Mat>( [&]()
                              {
                                return cv::Mat::zeros( getNumberOfJoints(), 1, CV_32F );
                              } 
                            ) );
  return true;
}

