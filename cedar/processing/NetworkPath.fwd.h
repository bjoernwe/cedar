/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        NetworkPath.fwd.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 11 19

    Description: Forward declaration file for the class cedar::proc::NetworkPath.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_NETWORK_PATH_FWD_H
#define CEDAR_PROC_NETWORK_PATH_FWD_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/lib.h"

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>

namespace cedar
{
  namespace proc
  {
    //!@todo The actual class should be called GroupPath
    //!@cond SKIPPED_DOCUMENTATION
    CEDAR_DECLARE_PROC_CLASS(NetworkPath);

    typedef NetworkPath GroupPath;
    CEDAR_GENERATE_POINTER_TYPES(GroupPath);
    //!@endcond
  }
}


#endif // CEDAR_PROC_NETWORK_PATH_FWD_H

