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

    File:        MatData.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 07 06

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/MatData.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/stringFunctions.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

std::string cedar::aux::MatData::getDescription() const
{
  std::string description;
  description += Super::getDescription();
  description += "<hr />";

  this->lockForRead();
  const cv::Mat& mat = this->getData();
  if (mat.empty())
  {
    description += "Empty matrix.";
  }
  else
  {
    unsigned int dim = cedar::aux::math::getDimensionalityOf(mat);
    description += cedar::aux::toString(dim) + "-dimensional matrix";
    if (dim != 0)
    {
      description += "<br />size: ";
      if (dim == 1)
      {
        description += cedar::aux::toString(cedar::aux::math::get1DMatrixSize(mat));
      }
      else
      {
        for (unsigned int i = 0; i < dim; ++i)
        {
          if (i > 0)
          {
            description += " x ";
          }
          description += cedar::aux::toString(mat.size[i]);
        }
      }
    }
    description += "<br />";
    description += "type: " + cedar::aux::math::matrixTypeToString(mat) + "<br />";
    description += "channels: " + cedar::aux::toString(mat.channels());
  }

  this->unlock();

  return description;
}
