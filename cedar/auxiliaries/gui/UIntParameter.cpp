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

    File:        UIntParameter.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 06

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/UIntParameter.h"
#include "cedar/auxiliaries/UIntParameter.h"
#include "cedar/auxiliaries/TypeBasedFactory.h"
#include "cedar/auxiliaries/Log.h"
#include "cedar/auxiliaries/Singleton.h"

// SYSTEM INCLUDES
#include <QHBoxLayout>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// associate aux::gui parameter with the aux parameter
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered = cedar::aux::gui::ParameterFactorySingleton::getInstance()->add
      <
        cedar::aux::UIntParameter,
        cedar::aux::gui::UIntParameter
      >();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::UIntParameter::UIntParameter(QWidget *pParent)
:
cedar::aux::gui::Parameter(pParent)
{
  this->setLayout(new QHBoxLayout());
  this->mpSpinbox = new QSpinBox();
  this->layout()->setContentsMargins(0, 0, 0, 0);
  this->layout()->addWidget(this->mpSpinbox);
  this->mpSpinbox->setMinimum(0.0);
  this->mpSpinbox->setMaximum(100.0);

  QObject::connect(this, SIGNAL(parameterPointerChanged()), this, SLOT(parameterPointerChanged()));
}

//!@brief Destructor
cedar::aux::gui::UIntParameter::~UIntParameter()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::gui::UIntParameter::parameterPointerChanged()
{
  cedar::aux::UIntParameterPtr parameter = boost::dynamic_pointer_cast<cedar::aux::UIntParameter>(this->getParameter());

  this->propertiesChanged();

  this->mpSpinbox->setValue(parameter->getValue());
  QObject::connect(this->mpSpinbox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

  QObject::connect(parameter.get(), SIGNAL(propertyChanged()), this, SLOT(propertiesChanged()));
}

void cedar::aux::gui::UIntParameter::propertiesChanged()
{
  cedar::aux::UIntParameterPtr parameter = boost::dynamic_pointer_cast<cedar::aux::UIntParameter>(this->getParameter());
  this->mpSpinbox->setMinimum(parameter->getMinimum());
  int maximum = static_cast<int>(parameter->getMaximum());
  if (maximum < 0)
  {
    cedar::aux::LogSingleton::getInstance()->warning
    (
      "Qt cannot display the value range given for parameter " + parameter->getName() + ". Defaulting to max int.",
      "cedar::aux::gui::UIntParameter::propertiesChanged()"
    );
    maximum = std::numeric_limits<int>::max();
  }
  this->mpSpinbox->setMaximum(maximum);
  this->mpSpinbox->setDisabled(parameter->isConstant());
}

void cedar::aux::gui::UIntParameter::valueChanged(int value)
{
  cedar::aux::UIntParameterPtr parameter = boost::dynamic_pointer_cast<cedar::aux::UIntParameter>(this->getParameter());
  parameter->setValue(value);
}
