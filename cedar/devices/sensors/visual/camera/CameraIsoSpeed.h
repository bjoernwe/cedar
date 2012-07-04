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

    File:        CameraIsoSpeed.h

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description:  Header for CameraIsoSpeed enum-type class

    Credits:

======================================================================================================================*/

#ifndef CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_ISO_SPEED_H
#define CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_ISO_SPEED_H

// CEDAR INCLUDES
#include "cedar/configuration.h"   // MAKE FIREWIRE OPTIONAL
#ifdef CEDAR_USE_LIB_DC1394

#include "cedar/auxiliaries/EnumType.h"
#include "cedar/devices/sensors/visual/namespace.h"

// SYSTEM INCLUDES


/*!@brief Enum class for firewire ISO-speed
 *
 * Use this type for the CameraGrabber::setCameraInitIsoSpeed() and getCameraInitIsoSpeed() method
 */
class cedar::dev::sensors::visual::CameraIsoSpeed
{
  //--------------------------------------------------------------------------------------------------------------------
  // typedefs
  //--------------------------------------------------------------------------------------------------------------------
//!@cond SKIPPED_DOCUMENTATION
public:
  typedef cedar::aux::EnumId Id;
public:
  typedef boost::shared_ptr<cedar::aux::EnumBase> TypePtr;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.

  //!@brief Destructor

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:  
  static void construct();

  static const cedar::aux::EnumBase& type();
  static const cedar::dev::sensors::visual::CameraIsoSpeed::TypePtr& typePtr(); 
  
  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  static cedar::aux::EnumType<cedar::dev::sensors::visual::CameraIsoSpeed> mType;
  //!@endcond
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:

  /*! @brief Allow OpenCV backend to set the ISO-speed of the firewire bus.
   *
   *  Set the ISO-speed manually with the CameraGrabber::setCameraIsoSpeed() method.
   *
   *  @remarks
   *  As every setting, this can only be done before the first frame was grabbed.
   */

  static const Id ISO_NOT_SET = UINT_MAX-2;

  /// @brief Set the ISO-speed to 100
  static const Id ISO_100 = 100;
  /// @see ISO_100
  static const Id ISO_200 = 200;
  /// @see ISO_100
  static const Id ISO_400 = 400;
  /// @see ISO_100
  static const Id ISO_800 = 800;
  /// @see ISO_100
  static const Id ISO_1600 = 1600;
  /// @see ISO_100
  static const Id ISO_3200 = 3200;

    
protected:
  // none yet
private:
  // none yet

}; // cedar::dev::sensors::visual::CameraIsoSpeed

#endif // CEDAR_USE_LIB_DC1394
#endif // CEDAR_CEDAR_DEV_SENSORS_VISUAL_CAMERA_ISO_SPEED_H

