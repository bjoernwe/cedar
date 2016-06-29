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

    File:        exceptions.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 28

    Description: Header file for exceptions in the cedar::aux::gui namespace.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GUI_EXCEPTIONS_H
#define CEDAR_AUX_GUI_EXCEPTIONS_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/ExceptionBase.h"

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/gui/exceptions.fwd.h"

// SYSTEM INCLUDES


/*!@brief Exception that occurs when a type is not handled.
 */
class cedar::aux::gui::InvalidPlotData : public cedar::aux::ExceptionBase
{
public:
  InvalidPlotData()
  {
  }
}; // class cedar::aux::InvalidPlotData

#endif // CEDAR_AUX_GUI_EXCEPTIONS_H
