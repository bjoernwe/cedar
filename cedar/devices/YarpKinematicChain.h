/*======================================================================================================================

 Copyright 2011, 2012, 2013, 2014, 2015 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

 File:        YarpKinematicChain.h

 Maintainer:  Jan Tekülve
 Email:       jan.tekuelve@ini.rub.de
 Date:        2016 06 20

 Description: Header file for the class cedar::dev::YarpKinematicChain.

 Credits:

 ======================================================================================================================*/

#ifndef CEDAR_DEV_YARP_KINEMATIC_CHAIN_H
#define CEDAR_DEV_YARP_KINEMATIC_CHAIN_H

// CEDAR INCLUDES
#include "cedar/devices/namespace.h"
#include "cedar/devices/KinematicChain.h"
#include "cedar/devices/YarpChannel.h"

// SYSTEM INCLUDES
#include <opencv2/opencv.hpp>
// FORWARD DECLARATIONS
#include "cedar/devices/YarpKinematicChain.fwd.h"

/*!@todo describe.
 *
 * @todo describe more.
 */
class cedar::dev::YarpKinematicChain : public cedar::dev::KinematicChain
{
Q_OBJECT
  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  YarpKinematicChain();

  //!@brief Destructor
  virtual ~YarpKinematicChain();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  bool applyCrashbrake();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:

  void postStart();
  void sendAngles(cv::Mat mat);
  cv::Mat retrieveAngles();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------

  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:

  cedar::aux::StringParameterPtr mReadPortName;
  cedar::aux::StringParameterPtr mWritePortName;

};
// class cedar::dev::YarpKinematicChain

#endif // CEDAR_DEV_YARP_KINEMATIC_CHAIN_H
