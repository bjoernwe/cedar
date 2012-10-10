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

    File:        MatrixTypeConverter.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 07 09

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/steps/MatrixTypeConverter.h"
#include "cedar/processing/ElementDeclaration.h"
#include "cedar/processing/DeclarationRegistry.h"
#include "cedar/auxiliaries/MatData.h"

// SYSTEM INCLUDES


//----------------------------------------------------------------------------------------------------------------------
// register the class
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool declare()
  {
    using cedar::proc::ElementDeclarationPtr;
    using cedar::proc::ElementDeclarationTemplate;

    ElementDeclarationPtr declaration
    (
      new ElementDeclarationTemplate<cedar::proc::steps::MatrixTypeConverter>
      (
        "Utilities",
        "cedar.processing.steps.MatrixTypeConverter"
      )
    );
    declaration->setIconPath(":/steps/matrix_type_conversion.svg");
    declaration->setDescription
    (
      "Converts a matrix from one type to another."
    );
    cedar::aux::Singleton<cedar::proc::DeclarationRegistry>::getInstance()->declareClass(declaration);

    return true;
  }

  bool declared = declare();
}

//----------------------------------------------------------------------------------------------------------------------
// static members
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::EnumType<cedar::proc::steps::MatrixTypeConverter::MatrixType>
  cedar::proc::steps::MatrixTypeConverter::MatrixType::mType("MatrixType::");

#ifndef CEDAR_COMPILER_MSVC
const cedar::proc::steps::MatrixTypeConverter::MatrixType::Id
  cedar::proc::steps::MatrixTypeConverter::MatrixType::Int8;
const cedar::proc::steps::MatrixTypeConverter::MatrixType::Id
  cedar::proc::steps::MatrixTypeConverter::MatrixType::UInt8;
const cedar::proc::steps::MatrixTypeConverter::MatrixType::Id
  cedar::proc::steps::MatrixTypeConverter::MatrixType::Float;
const cedar::proc::steps::MatrixTypeConverter::MatrixType::Id
  cedar::proc::steps::MatrixTypeConverter::MatrixType::Double;
#endif // CEDAR_COMPILER_MSVC

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::proc::steps::MatrixTypeConverter::MatrixTypeConverter()
:
// data
mConverted(new cedar::aux::MatData(cv::Mat(2, 2, CV_32F))),
// parameters
_mTargetType(new cedar::aux::EnumParameter(this, "target type", MatrixType::typePtr(), MatrixType::Float))
{
  this->declareInput("matrix");
  this->declareOutput("converted matrix", mConverted);

  QObject::connect(_mTargetType.get(), SIGNAL(valueChanged()), this, SLOT(targetTypeChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::steps::MatrixTypeConverter::targetTypeChanged()
{
  this->onTrigger();
}

void cedar::proc::steps::MatrixTypeConverter::compute(const cedar::proc::Arguments&)
{
  this->mMatrix->getData().convertTo(this->mConverted->getData(), this->getTargetType());
}

void cedar::proc::steps::MatrixTypeConverter::inputConnectionChanged(const std::string& dataSlotName)
{
  this->mMatrix = boost::dynamic_pointer_cast<const cedar::aux::MatData>(this->getInput(dataSlotName));

  if (!this->mMatrix)
  {
    return;
  }

  this->mConverted->copyAnnotationsFrom(this->mMatrix);

  this->onTrigger();
}

cedar::proc::DataSlot::VALIDITY cedar::proc::steps::MatrixTypeConverter::determineInputValidity
                                (
                                  cedar::proc::ConstDataSlotPtr, // this step only has one slot
                                  cedar::aux::ConstDataPtr data
                                ) const
{
  if (boost::dynamic_pointer_cast<const cedar::aux::MatData>(data))
  {
    return cedar::proc::DataSlot::VALIDITY_VALID;
  }
  else
  {
    return cedar::proc::DataSlot::VALIDITY_ERROR;
  }
}