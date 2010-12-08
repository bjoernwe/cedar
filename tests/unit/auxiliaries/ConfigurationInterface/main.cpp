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

    File:        main.cpp

    Maintainer:  Stephan Zibner
    Email:       stephan.zibner@rub.de
    Date:        2010 10 21

    Description: Constructs an object of a subclass of cedar::aux::ConfigurationInterface and lets it
                 read in several combinations of configuration parameters. The creation of new files is also checked.

    Credits:

======================================================================================================================*/

//!@todo write a function that deletes the files that are created in this test

// LOCAL INCLUDES
#include "TestClass.h"

// PROJECT INCLUDES
#include "cedar/auxiliaries/LogFile.h"

// SYSTEM INCLUDES
#include <iostream>

using namespace cedar::tests::unit::aux::ConfigurationInterface;
using namespace cedar::aux;

int main()
{
  LogFile log_file("ConfigurationInterface.log");
  log_file.addTimeStamp();
  log_file << std::endl;
  // the number of errors encountered in this test
  int errors = 0;
  // the current test number
  int test_number = 0;

  // check all add parameter functions
  // create test class part 1
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass test_standard("configDir/ConfigurationTest1.cfg");
  test_standard.addStandardParameters();
  test_standard.tryToReadManually();
  test_standard.checkConfigurationStandardParameters();
  test_standard.printErrors(log_file);
  errors += test_standard.errors();

  // create test class part 2
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass test_vectors_1("configDir/ConfigurationTest2.cfg");
  test_vectors_1.addVectorParametersSingleDefaults();
  test_vectors_1.tryToReadManually();
  test_vectors_1.checkConfigurationVectorParameters();
  test_vectors_1.printErrors(log_file);
  errors += test_vectors_1.errors();

  // create test class part 3
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass test_vectors_2("configDir/ConfigurationTest3.cfg");
  test_vectors_2.addVectorParametersMultipleDefaults();
  test_vectors_2.tryToReadManually();
  test_vectors_2.checkConfigurationVectorParameters();
  test_vectors_2.printErrors(log_file);
  errors += test_vectors_2.errors();

  // check if new file is created
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass new_file("configDir/ConfigurationNew.cfg");
  new_file.readEmptyFile();
  new_file.addStandardParameters();
  new_file.addVectorParametersMultipleDefaults();
  new_file.readOrDefaultConfiguration();
  new_file.checkConfigurationStandardParameters();
  new_file.checkConfigurationVectorParameters();
  new_file.printErrors(log_file);
  errors += new_file.errors();
  new_file.removeFile("configDir/ConfigurationNew.cfg");

  // check if new folder is created
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass new_folder("configNew/newConfig/ConfigurationNew.cfg");
  new_folder.readEmptyFileInNewFolder();
  new_folder.addStandardParameters();
  new_folder.addVectorParametersMultipleDefaults();
  new_folder.readOrDefaultConfiguration();
  new_folder.checkConfigurationStandardParameters();
  new_folder.checkConfigurationVectorParameters();
  new_folder.printErrors(log_file);
  errors += new_folder.errors();
  new_folder.removeDir("configNew");


  // check groups
  log_file.addSeparatorLine();
  log_file << "Test " << test_number++ << std::endl;
  TestClass new_group("configDir/ConfigurationGroupsNew.cfg");
  new_group.readEmptyFileInNewFolder();
  new_group.addGroupParameters();
  new_group.readOrDefaultConfiguration();
  new_group.checkConfigurationStandardParameters();
  new_group.checkConfigurationVectorParameters();
  new_group.printErrors(log_file);
  errors += new_group.errors();
  new_group.removeFile("configDir/ConfigurationGroupsNew.cfg");

  log_file << "test finished, there were " << errors << " errors" << std::endl;
  if (errors > 255)
  {
    errors = 255;
  }
  return errors;
}
