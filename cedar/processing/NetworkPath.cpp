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

    File:        NetworkPath.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 07 25

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/NetworkPath.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::NetworkPath::NetworkPath()
:
cedar::proc::NetworkPath::PathType()
{
}

cedar::proc::NetworkPath::NetworkPath(const cedar::proc::NetworkPath::String& path)
:
cedar::proc::NetworkPath::PathType(path)
{
}

cedar::proc::NetworkPath::NetworkPath(const char* path)
:
cedar::proc::NetworkPath::PathType(path)
{
}

cedar::proc::NetworkPath::NetworkPath(const PathType& path)
:
cedar::proc::NetworkPath::PathType(path)
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

std::string cedar::proc::NetworkPath::getElementName() const
{
  CEDAR_ASSERT(!this->empty());
  return this->operator[](this->getElementCount() - 1);
}
