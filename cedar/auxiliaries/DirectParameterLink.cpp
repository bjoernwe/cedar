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

    File:        DirectParameterLink.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 02 18

    Description: Source file for the class cedar::aux::DirectParameterLink.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/DirectParameterLink.h"
#include "cedar/auxiliaries/NumericParameter.h"
#include "cedar/auxiliaries/NumericParameterHelper.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// type registration
//----------------------------------------------------------------------------------------------------------------------

namespace
{
  bool registerType()
  {
    cedar::aux::ParameterLinkFactoryManagerSingleton::getInstance()->registerType<cedar::aux::DirectParameterLinkPtr>();
    return true;
  }

  bool registered = registerType();
}


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::DirectParameterLink::DirectParameterLink()
{
}

cedar::aux::DirectParameterLink::~DirectParameterLink()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::DirectParameterLink::leftChanged()
{
  this->assign(this->getLeft(), this->getRight());
}

void cedar::aux::DirectParameterLink::rightChanged()
{
  this->assign(this->getRight(), this->getLeft());
}

bool cedar::aux::DirectParameterLink::checkIfLinkable
     (
       cedar::aux::ConstParameterPtr /* left */,
       cedar::aux::ConstParameterPtr /* right */
     )
     const
{
  // TODO
  return true;
}


void cedar::aux::DirectParameterLink::assign
     (
       cedar::aux::ConstParameterPtr source,
       cedar::aux::ParameterPtr target
     ) const
{
  // if the target parameter can copy from the source, use this built-in functionality
  if (target->canCopyFrom(source))
  {
    target->copyValueFrom(source);
    return;
  }

  // otherwise, we have to check if we know how to convert these values appropriately

  if (cedar::aux::NumericParameterHelper::isNumeric(source) && cedar::aux::NumericParameterHelper::isNumeric(target))
  {
    double value = cedar::aux::NumericParameterHelper::getValue(source);
    cedar::aux::NumericParameterHelper::setValue(target, value);
  }
  else
  {
    // finally, if we couldn't handle the parameters until this point, throw an exception
    CEDAR_THROW
    (
      cedar::aux::UnhandledTypeException,
      "Cannot assing values from " + cedar::aux::objectTypeToString(source) + " to "
      + cedar::aux::objectTypeToString(target)
    );
  }
}
