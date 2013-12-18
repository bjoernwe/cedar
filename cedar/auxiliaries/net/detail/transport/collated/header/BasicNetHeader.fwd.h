/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        BasicNetHeader.fwd.h

    Maintainer:  jokeit
    Email:       jean-stephane.jokeit@ini.ruhr-uni-bochum.de
    Date:        2013 11 13

    Description: Forward declaration file for the class cedar::aux::net::detail::BasicNetHeader.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_NET_DETAIL_BASIC_NET_HEADER_FWD_H
#define CEDAR_AUX_NET_DETAIL_BASIC_NET_HEADER_FWD_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/lib.h"

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>

//!@cond SKIPPED_DOCUMENTATION
namespace cedar
{
  namespace aux
  {
    namespace net
    {
      namespace detail
      {
        struct CEDAR_AUX_LIB_EXPORT BasicNetHeader;
      }
    }
  }
}

//!@endcond

#endif // CEDAR_AUX_NET_DETAIL_BASIC_NET_HEADER_FWD_H

