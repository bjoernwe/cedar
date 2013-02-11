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

    File:        Locomotion.h

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 03 19

    Description: An object of this class represents the drive of a mobile robot.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_LOCOMOTION_H
#define CEDAR_DEV_LOCOMOTION_H

// CEDAR INCLUDES
#include "cedar/devices/namespace.h"
#include "cedar/devices/Component.h"
#include "cedar/auxiliaries/math/DoubleLimitsParameter.h"

// SYSTEM INCLUDES
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/velocity.hpp>
#include <boost/units/systems/si/angular_velocity.hpp>

/*!@brief An object of this class represents the locomotion of a mobile robot.
 *
 * This is an abstract class with functions and attributes common to drives of mobile robots. Mobile robots are e.g
 * robots with differential drives or walking robots.
 */
class cedar::dev::Locomotion : public cedar::dev::Component
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief Constructor
  Locomotion();

  //!@brief Constructor taking an externally created channel
  Locomotion(cedar::dev::ChannelPtr channel);

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief Returns the forward velocity [in m/s] of the robot.
  boost::units::quantity<boost::units::si::velocity> getForwardVelocity() const;

  //!@brief Returns the turning rate [in rad/s] of the robot.
  boost::units::quantity<boost::units::si::angular_velocity> getTurningRate() const;

  //!@brief Stops the robot.
  virtual void stop();

  /*!@ Returns the (estimated) current forward velocity [in m/s].
   *
   * This value is usually estimated based on sensors of the robot
   * (e.g., wheel encoders) and may differ from its real forward velocity.
   */
  //virtual double getEstimatedForwardVelocity() const = 0;

  /*!@ Returns the (estimated) current turning rate [in rad/s].
   *
   * This value is usually estimated based on sensors of the robot
   * (e.g., wheel encoders) and may differ from its real turning rate.
   */
  //virtual double getEstimatedTurningRate() const = 0;

  //!@brief Sets the current forward velocity [in m/s] of the robot.
  virtual void setForwardVelocity(boost::units::quantity<boost::units::si::velocity> forwardVelocity) = 0;

  //!@brief Sets the turning rate [in rad/s] of the robot.
  virtual void setTurningRate(boost::units::quantity<boost::units::si::angular_velocity> turningRate) = 0;

  //!@brief Sets both the forward velocity [in m/s] and turning rate [in rad/s] of the robot.
  virtual void setForwardVelocityAndTurningRate
               (
                 boost::units::quantity<boost::units::si::velocity> forwardVelocity,
                 boost::units::quantity<boost::units::si::angular_velocity> turningRate
               ) = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  /*!@brief Checks the given forward velocity against the configured limits and thresholds it if necessary.
   * @param[in,out] forwardVelocity forward velocity to be thresholded [m/s]
   */
  virtual void thresholdForwardVelocity(boost::units::quantity<boost::units::si::velocity>& forwardVelocity) const;

  /*!@brief Checks the given turning rate against the configured limits and thresholds it if necessary.
   * @param[in,out] turningRate turning rate to be thresholded [m/s]
   */
  virtual void thresholdTurningRate(boost::units::quantity<boost::units::si::angular_velocity>& turningRate) const;

  //!@brief Actually sends the movement commands to the robot.
  virtual void sendMovementCommand() = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! desired forward velocity [m/s]
  boost::units::quantity<boost::units::si::velocity> mForwardVelocity;
  //! desired turning rate of the robot [rad/s]
  boost::units::quantity<boost::units::si::angular_velocity> mTurningRate;

private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  //! limits for the forward velocity of the robot [m/s]
  cedar::aux::math::DoubleLimitsParameterPtr _mForwardVelocityLimits;
  //! limits for the turning rate of the robot [rad/s]
  cedar::aux::math::DoubleLimitsParameterPtr _mTurningRateLimits;

}; // class cedar::dev::Locomotion
#endif // CEDAR_DEV_LOCOMOTION_H
