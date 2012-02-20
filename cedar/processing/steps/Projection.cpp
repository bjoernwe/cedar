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
    Email:       mathis.richter@ini.rub.de
    Date:        2011 11 02

    Description: Processing step, which projects neuronal activation between processing steps of different
                 dimensionality.

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/steps/namespace.h"
#include "cedar/processing/steps/Projection.h"
#include "cedar/processing/ProjectionMappingParameter.h"
#include "cedar/processing/DataSlot.h"
#include "cedar/processing/Arguments.h"
#include "cedar/auxiliaries/MatData.h"
#include "cedar/auxiliaries/assert.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/MatrixIterator.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES
#include <iostream>
#include <vector>
#include <limits>

cedar::aux::EnumType<cedar::proc::steps::Projection::CompressionType>
  cedar::proc::steps::Projection::CompressionType::mType;

#ifndef CEDAR_COMPILER_MSVC
const cedar::proc::steps::Projection::CompressionType::Id cedar::proc::steps::Projection::CompressionType::SUM;
const cedar::proc::steps::Projection::CompressionType::Id cedar::proc::steps::Projection::CompressionType::AVERAGE;
const cedar::proc::steps::Projection::CompressionType::Id cedar::proc::steps::Projection::CompressionType::MAXIMUM;
const cedar::proc::steps::Projection::CompressionType::Id cedar::proc::steps::Projection::CompressionType::MINIMUM;
#endif // CEDAR_COMPILER_MSVC

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::proc::steps::Projection::Projection()
:
mInput(new cedar::aux::MatData(cv::Mat())),
mOutput(new cedar::aux::MatData(cv::Mat())),
_mDimensionMappings(new cedar::proc::ProjectionMappingParameter(this, "dimension mapping")),
_mOutputDimensionality(new cedar::aux::UIntParameter(this, "output dimensionality", 1, 0, 10)),
_mOutputDimensionSizes(new cedar::aux::UIntVectorParameter(this, "output dimension sizes", 1, 10, 1, 1000)),
_mCompressionType(new cedar::aux::EnumParameter(
                                                 this,
                                                 "compression type",
                                                 cedar::proc::steps::Projection::CompressionType::typePtr(),
                                                 cedar::proc::steps::Projection::CompressionType::SUM
                                               )
                  )
{
  // declare input and output
  this->declareInput("input");
  this->declareOutput("output", mOutput);

  // initialize the output buffer to the correct size
  this->outputDimensionalityChanged();

  // connect signals and slots
  QObject::connect(_mDimensionMappings.get(), SIGNAL(valueChanged()), this, SLOT(reconfigure()));
  QObject::connect(_mCompressionType.get(), SIGNAL(valueChanged()), this, SLOT(reconfigure()));
  QObject::connect(_mOutputDimensionality.get(), SIGNAL(valueChanged()), this, SLOT(outputDimensionalityChanged()));
  QObject::connect(_mOutputDimensionSizes.get(), SIGNAL(valueChanged()), this, SLOT(outputDimensionSizesChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::steps::Projection::onStart()
{
  this->_mDimensionMappings->setConstant(true);
}

void cedar::proc::steps::Projection::onStop()
{
  this->_mDimensionMappings->setConstant(false);
}

void cedar::proc::steps::Projection::compute(const cedar::proc::Arguments&)
{
  // call the appropriate projection method via the function pointer
  (this->*mpProjectionMethod)();
}

void cedar::proc::steps::Projection::outputDimensionalityChanged()
{
  // get the new output dimensionality
  unsigned int new_dimensionality = _mOutputDimensionality->getValue();

  // resize the dimensionality of the output buffer
  this->_mOutputDimensionSizes->resize(new_dimensionality, _mOutputDimensionSizes->getDefaultValue());

  // the number of mappings from input to output is constrained by the output dimensionality
  this->_mDimensionMappings->setOutputDimensionality(new_dimensionality);

  this->initializeOutputMatrix();

  this->reconfigure();
}

void cedar::proc::steps::Projection::outputDimensionSizesChanged()
{
  this->initializeOutputMatrix();
}

void cedar::proc::steps::Projection::reconfigure()
{
  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());
  unsigned int output_dimensionality = _mOutputDimensionality->getValue();

  // if the projection compresses ...
  if (input_dimensionality > output_dimensionality)
  {
    CEDAR_DEBUG_ASSERT(input_dimensionality == _mDimensionMappings->getValue()->getNumberOfMappings())

    // ... compute which indices need to be compressed
    mIndicesToCompress.clear();

    for (unsigned int index = 0; index < input_dimensionality; ++index)
    {
      if (_mDimensionMappings->getValue()->isDropped(index))
      {
        mIndicesToCompress.push_back(index);
      }
    }

    // set up the appropriate function pointer for different combinations of
    // input and output dimensionality
    if (input_dimensionality == 3 && output_dimensionality == 2)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress3Dto2D;
    }
    else if (input_dimensionality == 3 && output_dimensionality == 1)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress3Dto1D;
    }
    else if (input_dimensionality == 2 && output_dimensionality == 1)
    {
      mpProjectionMethod = &cedar::proc::steps::Projection::compress2Dto1D;
    }
    else if (output_dimensionality == 0)
    {
      if (_mCompressionType->getValue() == cedar::proc::steps::Projection::CompressionType::MAXIMUM)
      {
        mpProjectionMethod = &cedar::proc::steps::Projection::compressNDto0Dmax;
      }
      else if (_mCompressionType->getValue() == cedar::proc::steps::Projection::CompressionType::MINIMUM)
      {
        mpProjectionMethod = &cedar::proc::steps::Projection::compressNDto0Dmin;
      }
      else if (_mCompressionType->getValue() == cedar::proc::steps::Projection::CompressionType::AVERAGE)
      {
        mpProjectionMethod = &cedar::proc::steps::Projection::compressNDto0Dmean;
      }
      else if (_mCompressionType->getValue() == cedar::proc::steps::Projection::CompressionType::SUM)
      {
        mpProjectionMethod = &cedar::proc::steps::Projection::compressNDto0Dsum;
      }
    }
    else
    {
      std::cout << "Not implemented yet.\n";
    }
  }
  // if the projection expands ...
  else
  {
    // ... set up the appropriate function pointer
    if (input_dimensionality == 0)
    {
      this->mpProjectionMethod = &cedar::proc::steps::Projection::expand0DtoND;
    }
    else
    {
      this->mpProjectionMethod = &cedar::proc::steps::Projection::expandMDtoND;
    }
  }

  // @todo this should be handled by a more suitable mechanism for users
  if (this->_mDimensionMappings->getValue()->getValidity() == cedar::proc::ProjectionMapping::VALIDITY_ERROR)
  {
    this->setState(
                    cedar::proc::Step::STATE_EXCEPTION,
                    "The projection, as you have set it up, does not work in the given context.\
                    Please revise the mapping parameters."
                  );
  }
  else
  {
    this->setState(
                    cedar::proc::Step::STATE_NONE,
                    "Projection mapping is set up correctly."
                  );
  }
}

void cedar::proc::steps::Projection::initializeOutputMatrix()
{
  int dimensionality = static_cast<int>(_mOutputDimensionality->getValue());

  if (dimensionality == 0)
  {
    this->lockAll();
    this->mOutput->getData() = cv::Mat(1, 1, CV_32F, cv::Scalar(0));
    this->unlockAll();
  }
  else
  {
    // convert the sizes of the output dimensions to signed integers so that
    // OpenCV can handle it
    std::vector<int> sizes(dimensionality);
    for (int dim = 0; dim < dimensionality; ++dim)
    {
      sizes[dim] = _mOutputDimensionSizes->at(dim);
    }

    this->lockAll();
    this->mOutput->getData() = cv::Mat(dimensionality, &sizes.at(0), CV_32F, cv::Scalar(0));
    this->unlockAll();
  }
}

void cedar::proc::steps::Projection::expand0DtoND()
{
  CEDAR_DEBUG_ASSERT(mInput->getData().size[0] == 1)

  // set all values of the output matrix to the single value of the input
  mOutput->getData() = cv::Scalar(mInput->getData().at<float>(0));
}

//! not used at the moment
//!@todo check whether this is faster than the generic function
void cedar::proc::steps::Projection::expand1Dto2D()
{
  const cv::Mat& input = this->mInput->getData();
  cv::Mat& output = this->mOutput->getData();
  output = output * 0;

  bool transposed = false;

  // if dimension 0 is mapped onto 1
  if (_mDimensionMappings->getValue()->lookUp(0) == 1)
  {
    transposed = true;
    output = output.t();
  }

  for (int i = 0; i < input.rows; ++i)
  {
    input.row(i).copyTo(output);
  }

  if (transposed)
  {
    output = output.t();
  }
}

void cedar::proc::steps::Projection::expandMDtoND()
{
  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());

  // create an iterator for the output matrix
  cedar::aux::MatrixIterator output_iterator(mOutput->getData());

  // iterate through all elements of the output matrix
  do
  {
    // get index pointing to the current element in the output matrix
    const std::vector<int> output_index = output_iterator.getCurrentIndexVector();

    // compute the corresponding index in the input matrix
    std::vector<int> input_index;
    for (unsigned int i = 0; i < _mDimensionMappings->getValue()->getNumberOfMappings(); ++i)
    {
      input_index.push_back(output_index.at(_mDimensionMappings->getValue()->lookUp(i)));
    }

    // if the input dimensionality is 1 ...
    if (input_dimensionality == 1)
    {
      // ... we still need to have an index tuple because of OpenCV limitations
      input_index.push_back(0);
    }

    // copy the activation value in the input matrix to the corresponding output matrix
    mOutput->getData().at<float>(&(output_index.front()))
        = mInput->getData().at<float>(&(input_index.front()));
  }
  while (output_iterator.increment());
}

void cedar::proc::steps::Projection::compress2Dto1D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 1);

  cv::reduce(mInput->getData(), mOutput->getData(), mIndicesToCompress.at(0), _mCompressionType->getValue());

  if (mOutput->getData().rows == 1)
  {
    mOutput->getData() = mOutput->getData().t();
  }
}

void cedar::proc::steps::Projection::compress3Dto2D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 1);

  cedar::aux::math::reduceCvMat3D<float>(mInput->getData(), mOutput->getData(), mIndicesToCompress.at(0), _mCompressionType->getValue());
}

void cedar::proc::steps::Projection::compress3Dto1D()
{
  CEDAR_DEBUG_ASSERT(mIndicesToCompress.size() == 2);

  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());

  // the compression from 3D to 1D first compresses the input
  // cube to a temporary 2D matrix and then compresses that to 1D

  // in order to do that, we first have to find a vector containing
  // the sizes of the temporary 2D matrix ...
  std::vector<int> sizes;
  for (unsigned int i = 0; i < input_dimensionality; ++i)
  {
    if (i != mIndicesToCompress.at(0))
    {
      sizes.push_back(mInput->getData().size[i]);
    }
  }

  // ... and create the temporary 2D matrix with those sizes
  cv::Mat tmp_2d(input_dimensionality - 1, &sizes.front(), CV_32F, cv::Scalar(0.0));

  // reduce the 3D input to the (temporary) 2D matrix
  cedar::aux::math::reduceCvMat3D<float>(mInput->getData(), tmp_2d, mIndicesToCompress.at(0), _mCompressionType->getValue());

  // reduce the temporary 2D matrix to the final 1D output of the projection
  cv::reduce(tmp_2d, mOutput->getData(), mIndicesToCompress.at(1) - 1, _mCompressionType->getValue());

  if (mOutput->getData().rows == 1)
  {
    mOutput->getData() = mOutput->getData().t();
  }
}

void cedar::proc::steps::Projection::compressNDto0Dsum()
{
  mOutput->getData().at<float>(0) = cv::sum(mInput->getData())[0];
}

void cedar::proc::steps::Projection::compressNDto0Dmean()
{
  mOutput->getData().at<float>(0) = cv::mean(mInput->getData())[0];
}

void cedar::proc::steps::Projection::compressNDto0Dmin()
{
  double minimum = std::numeric_limits<double>::max();
  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());

  // for a dimensionality below 3 ...
  if (input_dimensionality < 3)
  {
    // ... we can use an OpenCV function to determine the minimum
    double maximum;
    cv::minMaxLoc(mInput->getData(), &minimum, &maximum);
  }
  // for all other dimensionalities, we have to iterate through the
  // input matrix and find the minimum ourselves
  else
  {
    // create an iterator for the input matrix
    cedar::aux::MatrixIterator matrix_iterator(mInput->getData());

    // iterate over input matrix and find the maximum value
    do
    {
      double current_value = mInput->getData().at<float>(matrix_iterator.getCurrentIndex());

      if (current_value < minimum)
      {
        minimum = current_value;
      }
    }
    while (matrix_iterator.increment());
  }

  // set the minimum of the input matrix as the output of the projection
  mOutput->getData().at<float>(0) = minimum;
}

void cedar::proc::steps::Projection::compressNDto0Dmax()
{
  double maximum = std::numeric_limits<double>::min();
  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());

  // for a dimensionality below 3 ...
  if (input_dimensionality < 3)
  {
    // ... we can use an OpenCV function to determine the maximum
    double minimum;
    cv::minMaxLoc(mInput->getData(), &minimum, &maximum);
  }
  // for all other dimensionalities, we have to iterate through the
  // input matrix and find the maximum ourselves
  else
  {
    // create an iterator for the input matrix
    cedar::aux::MatrixIterator matrix_iterator(mInput->getData());

    // iterate over input matrix and find the maximum value
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

  // set the maximum of the input matrix as the output of the projection
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

  this->mInput = boost::shared_dynamic_cast<const cedar::aux::MatData>(this->getInput(inputName));

  unsigned int input_dimensionality = cedar::aux::math::getDimensionalityOf(this->mInput->getData());

  if (input_dimensionality != this->_mDimensionMappings->getValue()->getNumberOfMappings())
  {
    this->_mDimensionMappings->initialize(input_dimensionality);
  }

  this->reconfigure();
}
