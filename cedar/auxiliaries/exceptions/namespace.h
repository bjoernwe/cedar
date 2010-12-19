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

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2010 10 19

    Description: Namespace file for cedar::aux::exc.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_EXC_NAMESPACE_H
#define CEDAR_AUX_EXC_NAMESPACE_H

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "namespace.h"

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>


namespace cedar
{
  namespace aux
  {
    //!@brief Namespace for all exception classes.
    namespace exc
    {
      class ExceptionBase;
      //!@brief smart pointer for ExceptionBase
      typedef boost::shared_ptr<ExceptionBase> ExceptionBasePtr;

      class NullPointerException;
      //!@brief smart pointer for NullPointerException
      typedef boost::shared_ptr<NullPointerException> NullPointerExceptionPtr;

      class IndexOutOfRangeException;
      //!@brief smart pointer for IndexOutOfRangeException
      typedef boost::shared_ptr<IndexOutOfRangeException> IndexOutOfRangePtr;
    }
  }
}

#endif // CEDAR_AUX_EXC_NAMESPACE_H
