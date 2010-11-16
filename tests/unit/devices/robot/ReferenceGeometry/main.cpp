/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        main.cpp

 ----- Author:      Mathis Richter
 ----- Email:       mathis.richter@ini.rub.de
 ----- Date:        2010 11 12

 ----- Description: Implements all unit tests for the @em cedar::dev::robot::ReferenceGeometry class.

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

#include "src/devices/robot/ReferenceGeometry.h"
#include <string>

using namespace cedar::dev::robot;

int main()
{
  ReferenceGeometry reference_geometry("test.conf");

  std::cout << "name: " << reference_geometry.getName() << "\n";

  return 0;
}
