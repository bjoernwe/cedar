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

    File:        Projection.cpp

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.ruhr-uni-bochum.de
    Date:        2011 11 02

    Description: Processing step, which projects neuronal activation between processing steps of different
                 dimensionality.

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/processing/steps/namespace.h"
#include "cedar/processing/steps/Projection.h"

// PROJECT INCLUDES
#include "cedar/auxiliaries/DataTemplate.h"
#include "cedar/auxiliaries/NumericParameter.h"
#include "cedar/auxiliaries/NumericVectorParameter.h"
#include "cedar/processing/DataSlot.h"
#include "cedar/auxiliaries/assert.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/processing/Arguments.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/MatrixIterator.h"

// SYSTEM INCLUDES
#include <iostream>
#include <vector>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::proc::steps::Projection::Projection()
:
mInput(new cedar::aux::MatData(cv::Mat())),
mOutput(new cedar::aux::MatData(cv::Mat())),
_mDimensionMappings(new cedar::aux::UIntVectorParameter(this, "dimension mapping", 1, 1, 0, 10)),
_mOutputDimensionality(new cedar::aux::UIntParameter(this, "output dimensionality", 1, 0, 10)),
_mOutputDimensionSizes(new cedar::aux::UIntVectorParameter(this, "output dimension sizes", 1, 10, 1, 1000)),
_mCompressionType(new cedar::aux::UIntParameter(this, "compression type", 0, 0, 3))
{
  this->declareInput("input");
  this->declareOutput("output", mOutput);

  this->initializeOutputMatrix();

  QObject::connect(_mDimensionMappings.get(), SIGNAL(valueChanged()), this, SLOT(reconfigure()));
  QObject::connect(_mOutputDimensionality.get(), SIGNAL(valueChanged()), this, SLOT(outputDimensionalityChanged()));
  QObject::connect(_mOutputDimensionSizes.get(), SIGNAL(valueChanged()), this, SLOT(outputDimensionSizesChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::steps::Projection::compute(const cedar::proc::Arguments&)
{
  (this->*mpProjectionMethod)();
}

void cedar::proc::steps::Projection::outputDimensionalityChanged()
{
  unsigned int new_dimensionality = _mOutputDimensionality->getValue();

  if (new_dimensionality == 0)
  {
    this->_mOutputDimensionSizes->resize(1, 1);
  }
  else
  {
    this->_mOutputDimensionSizes->resize(new_dimensionality, _mOutputDimensionSizes->getDefaultValue());
  }
  //this->_mDimensionMappings->setMaximum(new_dimensionality);
  this->initializeOutputMatrix();
}

void cedar::proc::steps::Projection::outputDimensionSizesChanged()
{
  this->initializeOutputMatrix();
}

void cedar::proc::steps::Projection::reconfigure()
{
  std::cout << "reconfigure\n";

  unsigned int output_dimensionality = _mOutputDimensionality->getValue();

  if (mInputDimensionality > output_dimensionality)
  {
    CEDAR_DEBUG_ASSERT(mInputDimensionality == _mDimensionMappings->getValue().size())

    mIndicesToCompress.clear();

    for (unsigned int index = 0; index < mInputDimensionality; ++index)
    {
      if (_mDimensionMappings->getValue().at(index) == 10)
      {
        mIndicesToCompress.push_back(index);
      }
    }

    std::cout << "number of indices to compress: " << mIndicesToCompress.size() << "\n";
    for (unsigned int index = 0; index < mIndicesToCompress.size(); ++index)
    {
      std::cout << mIndicesToCompress.at(index) << ", ";
    }

    if (mInputDimensionality == 3 && output_dimensionality == 2)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress3Dto2D;
    }
    else if (mInputDimensionality == 3 && output_dimensionality == 1)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress3Dto1D;
    }
    else if (mInputDimensionality == 2 && output_dimensionality == 1)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress2Dto1D;
    }
    else if (output_dimensionality == 0)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compressNDto0D;
    }
    else
    {
      std::cout << "Not implemented yet.\n";
    }
  }
  else
  {
    if (mInputDimensionality == 0)
    {
      this->mpProjectionMethod = &cedar::proc::steps::Projection::expand0DtoND;
    }
    else
    {
      this->mpProjectionMethod = &cedar::proc::steps::Projection::expandMDtoND;
    }
  }
}

void cedar::proc::steps::Projection::initializeOutputMatrix()
{
  int dimensionality = static_cast<int>(_mOutputDimensionality->getValue());

  if (dimensionality == 0)
  {
    dimensionality = 1;
  }

  std::vector<int> sizes(dimensionality);
  for (int dim = 0; dim < dimensionality; ++dim)
  {
    sizes[dim] = _mOutputDimensionSizes->at(dim);
  }

  this->lockAll();
  if (dimensionality == 1) // this includes dimensionality == 0
  {
    this->mOutput->getData() = cv::Mat(sizes[0], 1, CV_32F, cv::Scalar(0));
  }
  else
  {
    this->mOutput->getData() = cv::Mat(dimensionality, &sizes.at(0), CV_32F, cv::Scalar(0));
  }
  this->unlockAll();
}

void cedar::proc::steps::Projection::expand0DtoND()
{
  CEDAR_DEBUG_ASSERT(mInput->getData().size[0] == 1)

  mOutput->getData() = cv::Scalar(mInput->getData().at<float>(0));
}

/*
void cedar::proc::steps::Projection::expand1Dto2D()
{
  cv::Mat output = this->mOutput->getData();
  output = output * 0;

  bool transposed = false;

  if (// dimension 0 is mapped to 1)
  {
    transposed = true;
    output = output.t();
  }

  for (unsigned int i = 0; i < output.rows; ++i)
  {
    output.row(i).copyTo(input);
  }

  if (transposed)
  {
    output = output.T();
  }
}
*/

void cedar::proc::steps::Projection::expandMDtoND()
{
  std::cout << "expanding!\n";
  const cv::Mat& input = mInput->getData();
  cv::Mat& output = mOutput->getData();

  std::vector<int> indices;

  for (unsigned int i = 0; i < _mOutputDimensionality->getValue(); ++i)
  {
    indices.push_back(0);
  }

  while (indices.back() < static_cast<int>(_mOutputDimensionSizes->back()))
  {
    std::vector<int> input_indices;

    for (unsigned int i = 0; i < _mDimensionMappings->size(); ++i)
    {
      std::cout << "mapping: " << i << " -> " << _mDimensionMappings->at(i) << "\n";
      input_indices.push_back(indices.at(_mDimensionMappings->at(i)));
    }

    if (mInputDimensionality == 1)
    {
      input_indices.push_back(0);
    }

    if (_mOutputDimensionality->getValue() == 1)
    {
      indices.push_back(0);
    }

    output.at<float>(&(indices.front()))
        = input.at<float>(&(input_indices.front()));

    if (_mOutputDimensionality->getValue() == 1)
    {
      indices.pop_back();
    }

    indices.front() += 1;

    for (unsigned int i = 0; i < indices.size() - 1; ++i) // the last index doesn't overflow
    {
      if (indices.at(i) >= static_cast<int>(_mOutputDimensionSizes->at(i)))
      {
        indices.at(i) = 0;
        indices.at(i + 1) += 1;
      }
      else
      {
        break;
      }
    }
  }
}

void cedar::proc::steps::Projection::compress2Dto1D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 1);

  cv::reduce(mInput->getData(), mOutput->getData(), mIndicesToCompress.at(0), _mCompressionType->getValue());
}

void cedar::proc::steps::Projection::compress3Dto2D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 1);

  cedar::aux::math::reduceCvMat3D<float>(mInput->getData(), mOutput->getData(), mIndicesToCompress.at(0), _mCompressionType->getValue());
}

void cedar::proc::steps::Projection::compress3Dto1D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 2);

  std::vector<int> sizes;

  for (unsigned int i = 0; i < mInputDimensionality; ++i)
  {
    if (i != mIndicesToCompress.at(0))
    {
      sizes.push_back(mInput->getData().size[i]);
    }
  }

  cv::Mat tmp_2d(mInputDimensionality - 1, &sizes.front(), CV_32F, cv::Scalar(0.0));

  cedar::aux::math::reduceCvMat3D<float>(mInput->getData(), tmp_2d, mIndicesToCompress.at(0), _mCompressionType->getValue());
  cv::reduce(tmp_2d, mOutput->getData(), mIndicesToCompress.at(1) - 1, _mCompressionType->getValue());
}

void cedar::proc::steps::Projection::compressNDtoMD()
{
  // iterate over output matrix
  // map current indices to indices in the input matrix
  // fill the indices, which are to be compressed, with zeros
  // from the input matrix get the "maximum" along all dimensions, which are to be compressed

}

void cedar::proc::steps::Projection::compressNDto0D()
{
  double maximum = 0;

  if (mInputDimensionality < 3)
  {
    double minimum;
    cv::minMaxLoc(mInput->getData(), &minimum, &maximum);
  }
  else
  {
    cedar::aux::MatrixIterator matrix_iterator(mInput->getData());

    do
    {
      double current_value = mInput->getData().at<float>(matrix_iterator.getCurrentIndex());

      if (current_value > maximum)
      {
        maximum = current_value;
      }
    }
    while (matrix_iterator.increment());
  }

  mOutput->getData().at<float>(0) = maximum;
}

cedar::proc::DataSlot::VALIDITY cedar::proc::steps::Projection::determineInputValidity
                                (
                                  cedar::proc::ConstDataSlotPtr CEDAR_DEBUG_ONLY(slot),
                                  cedar::aux::DataPtr data
                                ) const
{
  CEDAR_DEBUG_ASSERT(slot->getName() == "input")
  if (boost::shared_dynamic_cast<cedar::aux::MatData>(data))
  {
    return cedar::proc::DataSlot::VALIDITY_VALID;
  }
  else
  {
    return cedar::proc::DataSlot::VALIDITY_ERROR;
  }
}

void cedar::proc::steps::Projection::inputConnectionChanged(const std::string& inputName)
{
  CEDAR_DEBUG_ASSERT(inputName == "input");

  this->mInput = boost::shared_dynamic_cast<cedar::aux::MatData>(this->getInput(inputName));
  mInputDimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());
  this->_mDimensionMappings->resize(mInputDimensionality, _mDimensionMappings->getDefaultValue());

  this->reconfigure();
}
