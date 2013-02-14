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

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 03 21

    Description: This test checks the functionality of the UnitParameter.

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/LengthParameter.h"
#include "cedar/auxiliaries/TimeParameter.h"

// SYSTEM INCLUDES
#include <boost/filesystem.hpp>


class TestConfigurable : public cedar::aux::Configurable
{
public:
  TestConfigurable()
  :
  mLength
  (
    new cedar::aux::LengthParameter
    (
      this,
      "length",
      0 * boost::units::si::meters
    )
  ),
  mTime
  (
    new cedar::aux::TimeParameter
    (
      this,
      "time",
      0 * boost::units::si::seconds
    )
  )
  {
  }

  cedar::aux::LengthParameterPtr mLength;
  cedar::aux::TimeParameterPtr mTime;
};

CEDAR_GENERATE_POINTER_TYPES(TestConfigurable);


template <typename T>
int check(const boost::units::quantity<T>& expected, boost::intrusive_ptr<cedar::aux::UnitParameterTemplate<T> > param)
{
  if (param->getValue() != expected)
  {
    std::cout << "<<ERROR>>: Wrong unit was read for parameter \"" << param->getName() << "\": expected " << expected
              << ", read: " << param->getValue() << std::endl;
    return 1;
  }
  else
  {
    std::cout << "Parameter \"" << param->getName() << "\" was read correctly ("
              << param->getValue() << " == " << expected << ")." << std::endl;
    return 0;
  }
}

int test_reading
    (
      const std::string& fileName,
      const boost::units::quantity<boost::units::si::length>& expectedLength,
      const boost::units::quantity<boost::units::si::time>& expectedTime
    )
{
  int errors = 0;
  std::cout << "Testing reading of file \"" << fileName << "\"." << std::endl;

  {
    TestConfigurablePtr conf(new TestConfigurable());

    std::cout << "Reading of file \"" << fileName << "\" finished with " << errors << " error(s)." << std::endl;
    conf->readJson(fileName);

    errors += check(expectedLength, conf->mLength);
    errors += check(expectedTime, conf->mTime);
  }
  return errors;
}

int test_writing(const std::string& fileName)
{
  int errors = 0;
  std::cout << "Testing reading of file \"" << fileName << "\"." << std::endl;

  TestConfigurablePtr conf(new TestConfigurable());
  conf->writeJson(fileName);

  boost::filesystem::remove(fileName.c_str());

  std::cout << "Writing of file \"" << fileName << "\" finished with " << errors << " error(s)." << std::endl;
  return errors;
}

int main(int, char**)
{
  int errors = 0;

  errors += test_reading("test1-read.json", 1.0 * boost::units::si::meters, 5.0 * boost::units::si::seconds);
  errors += test_reading("test2-read.json", 1.0 * boost::units::si::meters, 5.0 * boost::units::si::seconds);
  errors += test_reading("test3-read.json", 1.0 * boost::units::si::meters, 5.0 * boost::units::si::seconds);
  errors += test_writing("test1-write.json");

  std::cout << "Test finished with " << errors << " error(s)." << std::endl;
  return errors;
}
