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

    File:        Noise.h

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 11 10

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/dynamics/sources/Noise.h"

// PROJECT INCLUDES
#include "cedar/auxiliaries/NumericParameter.h"
#include "cedar/auxiliaries/NumericVectorParameter.h"
#include "cedar/auxiliaries/DataTemplate.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::dyn::Noise::Noise()
:
mRandomMatrix(new cedar::aux::MatData(cv::Mat::zeros(10,10,CV_32F))),
_mDimensionality(new cedar::aux::UIntParameter(this, "dimensionality", 0, 1000)),
_mSizes(new cedar::aux::UIntVectorParameter(this, "sizes", 2, 10, 1, 1000))
{
  _mDimensionality->setValue(2);
  _mSizes->makeDefault();
  QObject::connect(_mSizes.get(), SIGNAL(valueChanged()), this, SLOT(dimensionSizeChanged()));
  QObject::connect(_mDimensionality.get(), SIGNAL(valueChanged()), this, SLOT(dimensionalityChanged()));
  this->declareOutput("random");
  this->setOutput("random", mRandomMatrix);

  // now check the dimensionality and sizes of all matrices
  this->updateMatrices();
}
//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------
void cedar::dyn::Noise::eulerStep(const cedar::unit::Time&)
{
  cv::Mat& random = this->mRandomMatrix->getData();

  // one possible preshape dynamic
  cv::randn(random, cv::Scalar(0), cv::Scalar(1));
}

void cedar::dyn::Noise::dimensionalityChanged()
{
  this->_mSizes->resize(_mDimensionality->getValue(), _mSizes->getDefaultValue());
  this->updateMatrices();
}

void cedar::dyn::Noise::dimensionSizeChanged()
{
  this->updateMatrices();
}

void cedar::dyn::Noise::updateMatrices()
{
  int dimensionality = static_cast<int>(_mDimensionality->getValue());

  std::vector<int> sizes(dimensionality);
  for (int dim = 0; dim < dimensionality; ++dim)
  {
    sizes[dim] = _mSizes->at(dim);
  }
  this->lockAll();
  if (dimensionality == 0)
  {
    this->mRandomMatrix->getData() = cv::Mat(1, 1, CV_32F, cv::Scalar(0));
  }
  else if (dimensionality == 1)
  {
    this->mRandomMatrix->getData() = cv::Mat(sizes[0], 1, CV_32F, cv::Scalar(0));
  }
  else
  {
    this->mRandomMatrix->getData() = cv::Mat(dimensionality,&sizes.at(0), CV_32F, cv::Scalar(0));
  }
  this->unlockAll();
}
