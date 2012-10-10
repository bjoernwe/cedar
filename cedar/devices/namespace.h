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

    File:        namespace.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 04 13

    Description: Namespace file for cedar::dev.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_NAMESPACE_H
#define CEDAR_DEV_NAMESPACE_H

// CEDAR INCLUDES
#include "cedar/devices/lib.h"
#include "cedar/defines.h"

// SYSTEM INCLUDES


namespace cedar
{
  /*!@brief Namespace for all dev classes. */
  namespace dev
  {
    //!@cond SKIPPED_DOCUMENTATION
    CEDAR_DECLARE_DEV_CLASS(UnresponsiveRobotException);
    CEDAR_DECLARE_DEV_CLASS(SerialCommunicationException);
    CEDAR_DECLARE_DEV_CLASS(UnknownOperatingSystemException);
    //!@endcond
  }
}

#endif // CEDAR_DEV_NAMESPACE_H