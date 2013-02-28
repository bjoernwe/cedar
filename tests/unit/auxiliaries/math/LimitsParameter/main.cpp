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

    File:        main.cpp

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2012 03 21

    Description: Unit tests for the LimitsParameter class.

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/math/LimitsParameter.h"
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/math/tools.h"

// SYSTEM INCLUDES

class TestClass : public cedar::aux::Configurable
{
public:
  TestClass()
  :
  _mpLimitsParameter(new cedar::aux::math::LimitsParameter<double>(this, "limits", -10.0, 1.0, -1.0, 10.0))
  {
  }

  double getLowerLimit() const
  {
    return this->_mpLimitsParameter->getLowerLimit();
  }

  double getUpperLimit() const
  {
    return this->_mpLimitsParameter->getUpperLimit();
  }

  void setLowerLimit(double value)
  {
    this->_mpLimitsParameter->setLowerLimit(value);
  }

  void setUpperLimit(double value)
  {
    this->_mpLimitsParameter->setUpperLimit(value);
  }
  cedar::aux::math::DoubleLimitsParameterPtr getLimitsParameter()
  {
    return this->_mpLimitsParameter;
  }

private:
  cedar::aux::math::DoubleLimitsParameterPtr _mpLimitsParameter;
};

CEDAR_GENERATE_POINTER_TYPES(TestClass);

int main()
{
  // the number of errors encountered in this test
  int number_of_errors = 0;

  TestClassPtr limited(new TestClass);

  // read from file
  limited->readJson("limits.json");
  if (!(cedar::aux::math::isZero<double>(limited->getLowerLimit() - (-2.0))))
  {
    std::cout << "lower limit was not read correctly, read:" << limited->getLowerLimit() << std::endl;
    ++number_of_errors;
  }
  if (!(cedar::aux::math::isZero<double>(limited->getUpperLimit() - (2.2))))
  {
    std::cout << "upper limit was not read correctly, read:" << limited->getUpperLimit()<< std::endl;
    ++number_of_errors;
  }

  // check extrema
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getLowerLimitMinimum() - (-10.0))))
  {
    std::cout << "lower limit minimum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getLowerLimitMaximum() - (1.0))))
  {
    std::cout << "lower limit maximum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getUpperLimitMinimum() - (-1.0))))
  {
    std::cout << "upper limit minimum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getUpperLimitMaximum() - (10.0))))
  {
    std::cout << "upper limit maximum was not set correctly" << std::endl;
    ++number_of_errors;
  }

  // check set functions
  limited->setLowerLimit(-5.6);
  if (!(cedar::aux::math::isZero<double>(limited->getLowerLimit() - (-5.6))))
  {
    std::cout << "lower limit was not set correctly" << std::endl;
    ++number_of_errors;
  }
  limited->setUpperLimit(6.7);
  if (!(cedar::aux::math::isZero<double>(limited->getUpperLimit() - (6.7))))
  {
    std::cout << "upper limit was not set correctly" << std::endl;
    ++number_of_errors;
  }
  limited->getLimitsParameter()->setLowerLimitMinimum(-12.3);
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getLowerLimitMinimum() - (-12.3))))
  {
    std::cout << "lower limit minimum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  limited->getLimitsParameter()->setLowerLimitMaximum(3.4);
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getLowerLimitMaximum() - (3.4))))
  {
    std::cout << "lower limit maximum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  limited->getLimitsParameter()->setUpperLimitMinimum(-4.5);
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getUpperLimitMinimum() - (-4.5))))
  {
    std::cout << "upper limit minimum was not set correctly" << std::endl;
    ++number_of_errors;
  }
  limited->getLimitsParameter()->setUpperLimitMaximum(6.7);
  if (!(cedar::aux::math::isZero<double>(limited->getLimitsParameter()->getUpperLimitMaximum() - (6.7))))
  {
    std::cout << "upper limit maximum was not set correctly" << std::endl;
    ++number_of_errors;
  }



  return number_of_errors;
}
