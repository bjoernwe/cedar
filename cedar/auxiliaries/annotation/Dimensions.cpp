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

    File:        Dimensions.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 06 29

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/annotation/Dimensions.h"
#include "cedar/auxiliaries/stringFunctions.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::annotation::Dimensions::Dimensions(unsigned int numberOfDimensions)
{
  this->mDimensions.resize(numberOfDimensions);
}

cedar::aux::annotation::Dimensions::Dimensions(const cedar::aux::annotation::Dimensions& copyFrom)
:
cedar::aux::annotation::Annotation(copyFrom)
{
  this->mDimensions.resize(copyFrom.getDimensionality());
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::annotation::Dimensions::setLabel(unsigned int dimension, const std::string& label)
{
  CEDAR_ASSERT(dimension < this->mDimensions.size());

  this->mDimensions[dimension].mLabel = label;
}

const std::string& cedar::aux::annotation::Dimensions::getLabel(unsigned int dimension) const
{
  CEDAR_ASSERT(dimension < this->mDimensions.size());

  return this->mDimensions[dimension].mLabel;
}

std::string cedar::aux::annotation::Dimensions::getDescription() const
{
  std::string description;

  for (size_t i = 0; i < this->mDimensions.size(); ++i)
  {
    description += "Dimension " + cedar::aux::toString(i) + ": " + this->mDimensions[i].mLabel;
  }

  return description;
}
