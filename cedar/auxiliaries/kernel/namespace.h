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

    File:        namespace.h

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.rub.de
    Date:        2011 07 14

    Description: Namespace file for cedar::aux::kernel.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_KERNEL_NAMESPACE_H
#define CEDAR_AUX_KERNEL_NAMESPACE_H

// LOCAL INCLUDES
#include "namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>

namespace cedar
{
  /*!@brief Namespace for all aux classes. */
  namespace aux
  {
  /*!@brief Namespace for all kernel classes. */
    namespace kernel
    {
      class Kernel;
      //!@brief smart pointer for Kernel
      typedef boost::shared_ptr<Kernel> KernelPtr;

      class Separable;
      //!@brief smart pointer for Separable
      typedef boost::shared_ptr<Separable> SeparablePtr;

      class Gauss;
      //!@brief smart pointer for Gauss
      typedef boost::shared_ptr<Gauss> GaussPtr;
    }
  }
}

#endif // CEDAR_AUX_KERNEL_NAMESPACE_H
