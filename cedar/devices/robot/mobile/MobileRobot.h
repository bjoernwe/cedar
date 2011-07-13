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

    File:        MobileRobot.h

    Maintainer:  Andre Bartel
    Email:       andre.bartel@ini.ruhr-uni-bochum.de
    Date:        2011 03 19

    Description: An object of this class represents the drive of a mobile robot.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_ROBOT_MOBILE_MOBILE_ROBOT_H
#define CEDAR_DEV_ROBOT_MOBILE_MOBILE_ROBOT_H

// LOCAL INCLUDES

#include "devices/robot/mobile/namespace.h"
#include "devices/robot/Component.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

/*!@brief An object of this class represents the drive of a mobile robot.
 *
 * This is an abstract class with functions and attributes common to drives of mobile robots. Mobile robots are e.g
 * robots with differential drives or walking robots.
 */
class cedar::dev::robot::mobile::MobileRobot : public cedar::dev::robot::Component
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------

public:

  //!@brief Constructs the object which represents the drive of a mobile robot.
  MobileRobot();

  //!@brief Destructs the object.
  virtual ~MobileRobot() = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------

public:

  //!@brief The get-function of both forward velocity and turning rate.
  //!@return 2-dimensional vector with mForwardVelocity [in m/s] as 1st and mTurningRate in [rad/s] as 2nd element.
  std::vector<double> getVelocity();

  //!@brief The get-function of the forward velocity.
  //!@return Forward Velocity [in m/s]
  double getForwardVelocity();

  //!@brief The get-function of the turning rate.
  //!@return Turning Rate [in rad/s]
  double getTurningRate();

  /*!@brief Sets both forward velocity and turning rate.
   *@param forwardVelocity The forward velocity to be set [in m/s].
   *@param turningRate The turning rate to be set [in rad/s].
   *@return 1 if setting forward velocity and turning rate was successful and 0 otherwise.
   */
  virtual int setVelocity(double forwardVelocity, double turningRate) = 0;

  /*!@brief Sets forward velocity only.
   *@param forwardVelocity The forward velocity to be set [in m/s].
   *@return 1 if setting forward velocity was successful and 0 otherwise.
   */
  virtual int setForwardVelocity(double forwardVelocity) = 0;

  /*!@brief Sets turning rate only.
   *@param turningRate The turning rate to be set [in rad/s].
   *@return 1 if setting turning rate was successful and 0 otherwise.
   */
  virtual int setTurningRate(double turningRate) = 0;

  /*!@brief Stops the robot.
   *@return 1 if stopping the robot was successful and 0 otherwise.
   */
  int stop();

  /*!@brief Resets the robot.
   *@return 1 if resetting the robot was successful and 0 otherwise.
   *
   *This function stops the robot and resets its movement-sensors (e.g. encoders).
   */
  virtual int reset() = 0;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------

protected:

  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------

private:

  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------

public:

  // none yet (hopefully never!)

protected:

  //!@brief Vector with forward velocity as 1st and turning rate as 2nd element [both in m/s].
  std::vector<double> mVelocity;

private:

  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------

public:

  // none yet (hopefully never!)

protected:

  /*!@brief The Debug-Flag.
   *If true, error-messages and received strings from the robot are displayed on Console, else not.
   */
  bool _mDebug;

private:

  // none yet

}; // class cedar::dev::robot::mobile::MobileRobot

#endif // CEDAR_DEV_ROBOT_MOBILE_MOBILE_ROBOT_H
