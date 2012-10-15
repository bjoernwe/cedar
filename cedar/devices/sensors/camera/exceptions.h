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

    File:        Exceptions.h

    Maintainer:  Georg.Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: Header file for all new exceptions introduced in the @em cedar::dev::sensors::visual namespace.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_DEV_SENSORS_VISUAL_EXCEPTIONS_H
#define CEDAR_DEV_SENSORS_VISUAL_EXCEPTIONS_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/ExceptionBase.h"
#include "cedar/devices/sensors/camera/namespace.h"

/*!@brief Exception on an error while initializing the LibDc class
 */
class cedar::dev::sensors::camera::LibDcInitException : public cedar::aux::ExceptionBase
{
};

/*!@brief Exception on an error while using the LibDc class
 */
class cedar::dev::sensors::camera::LibDcException : public cedar::aux::ExceptionBase
{
};




#endif /* CEDAR_DEV_SENSORS_VISUAL_EXCEPTIONS_H */
