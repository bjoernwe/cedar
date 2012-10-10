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

    File:        CameraDeviceVfl.cpp

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2012 07 04

    Description:  Implementation for the cedar::dev::sensors::visual::CameraDeviceVfl class

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

#ifdef CEDAR_USE_VIDEO_FOR_LINUX

// CEDAR INCLUDES
#include "cedar/devices/sensors/visual/camera/backends/CameraDeviceVfl.h"


// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::dev::sensors::visual::CameraDeviceVfl::CameraDeviceVfl
(
  cedar::dev::sensors::visual::CameraGrabber* pCameraGrabber,
  cedar::dev::sensors::visual::CameraChannelPtr pCameraChannel
)
:
cedar::dev::sensors::visual::CameraDevice::CameraDevice(pCameraGrabber,pCameraChannel)
{
}


cedar::dev::sensors::visual::CameraDeviceVfl::~CameraDeviceVfl()
{
}


//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::sensors::visual::CameraDeviceVfl::setProperties()
{

}

bool cedar::dev::sensors::visual::CameraDeviceVfl::createCaptureDevice()
{

}

void cedar::dev::sensors::visual::CameraDeviceVfl::applySettingsToCamera()
{

}

void cedar::dev::sensors::visual::CameraDeviceVfl::applyStateToCamera()
{

}

#endif // CEDAR_USE_VIDEO_FOR_LINUX