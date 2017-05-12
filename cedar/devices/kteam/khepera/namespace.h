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

    File:        namespace.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 11 26

    Description: Namespace file for cedar::dev::kteam::khepera.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_KTEAM_KHEPERA_NAMESPACE_H
#define CEDAR_DEV_KTEAM_KHEPERA_NAMESPACE_H

// CEDAR INCLUDES
#include "cedar/devices/lib.h"

// SYSTEM INCLUDES
#ifndef Q_MOC_RUN
  #include <boost/smart_ptr.hpp>
#endif // Q_MOC_RUN

namespace cedar
{
  namespace dev
  {
    namespace kteam
    {
      namespace khepera
      {
        //!@cond SKIPPED_DOCUMENTATION
        CEDAR_DECLARE_DEV_CLASS(Gripper);
        CEDAR_DECLARE_DEV_CLASS(GripperSerial);
        //!@endcond
      }
    }
  }
}

#endif // CEDAR_DEV_KTEAM_KHEPERA_NAMESPACE_H
