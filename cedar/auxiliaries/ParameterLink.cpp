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

    File:        ParameterLink.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2014 02 18

    Description: Source file for the class cedar::aux::ParameterLink.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/ParameterLink.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::ParameterLink::ParameterLink()
{
}

cedar::aux::ParameterLink::~ParameterLink()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::ParameterLink::setSource(cedar::aux::ParameterPtr parameter)
{
  if (this->mSource)
  {
    this->mSource->setLinked(false);

    QObject::disconnect(this->mSource.get(), SIGNAL(valueChanged()), this, SLOT(sourceChanged()));
    QObject::disconnect(this->mSource.get(), SIGNAL(propertyChanged()), this, SLOT(sourcePropertiesChanged()));
  }

  this->mSource = parameter;
  this->mSource->setLinked(true);
  QObject::connect(this->mSource.get(), SIGNAL(valueChanged()), this, SLOT(sourceChanged()));
  QObject::connect(this->mSource.get(), SIGNAL(propertyChanged()), this, SLOT(sourcePropertiesChanged()));
}

void cedar::aux::ParameterLink::setTarget(cedar::aux::ParameterPtr parameter)
{
  // disconnect if parameters are already set
  if (this->mTarget)
  {
    this->mTarget->setLinked(false);

    QObject::disconnect(this->mTarget.get(), SIGNAL(valueChanged()), this, SLOT(targetChanged()));
    QObject::disconnect(this->mTarget.get(), SIGNAL(propertyChanged()), this, SLOT(targetPropertiesChanged()));
  }

  this->mTarget = parameter;
  this->mTarget->setLinked(true);

  QObject::connect(this->mTarget.get(), SIGNAL(valueChanged()), this, SLOT(targetChanged()));
  QObject::connect(this->mTarget.get(), SIGNAL(propertyChanged()), this, SLOT(targetPropertiesChanged()));
}

void cedar::aux::ParameterLink::setLinkedParameters(cedar::aux::ParameterPtr source, cedar::aux::ParameterPtr target)
{
  //!@todo Exception; also, this should be checked in setSource, setTarget as well
  CEDAR_ASSERT(this->canLink(source, target));

  this->setSource(source);
  this->setTarget(target);
}

void cedar::aux::ParameterLink::sourcePropertiesChanged()
{
  this->applyProperties(this->getSource(), this->getTarget());
}

void cedar::aux::ParameterLink::targetPropertiesChanged()
{
  this->applyProperties(this->getTarget(), this->getSource());
}

void cedar::aux::ParameterLink::applyProperties(cedar::aux::ConstParameterPtr source, cedar::aux::ParameterPtr target)
{
  target->setConstant(source->isConstant());
}

cedar::aux::ParameterPtr cedar::aux::ParameterLink::getSource() const
{
  return this->mSource;
}

cedar::aux::ParameterPtr cedar::aux::ParameterLink::getTarget() const
{
  return this->mTarget;
}

bool cedar::aux::ParameterLink::canLink(cedar::aux::ParameterPtr source, cedar::aux::ParameterPtr target)
{
  return this->checkIfLinkable(source, target);
}
