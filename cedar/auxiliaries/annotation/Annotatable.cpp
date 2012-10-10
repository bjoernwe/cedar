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

    File:        Annotatable.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 07 06

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/annotation/Annotatable.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/utilities.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::annotation::Annotatable::~Annotatable()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::annotation::Annotatable::setAnnotation(cedar::aux::annotation::AnnotationPtr annotation)
{
  try
  {
    size_t i = this->findAnnotation(annotation);
    this->mAnnotations[i] = annotation;
  }
  catch (cedar::aux::AnnotationNotFoundException&)
  {
    this->mAnnotations.push_back(annotation);
  }
}

void cedar::aux::annotation::Annotatable::copyAnnotationsFrom(cedar::aux::annotation::ConstAnnotatablePtr other)
{
  this->mAnnotations.resize(other->mAnnotations.size());
  for (size_t i = 0; i < other->mAnnotations.size(); ++i)
  {
    this->mAnnotations[i] = other->mAnnotations[i]->clone();
  }
}

std::string cedar::aux::annotation::Annotatable::getDescription() const
{
  std::string description;
  description += "Type: <b>" + cedar::aux::objectTypeToString(this) + "</b>";

  for (size_t i = 0; i < this->mAnnotations.size(); ++i)
  {
    description += "<hr />";
    description += this->mAnnotations[i]->getDescription();
  }

  return description;
}