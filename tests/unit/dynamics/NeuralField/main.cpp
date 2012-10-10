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

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 03

    Description:

    Credits:

======================================================================================================================*/


// LOCAL INCLUDES

// PROJECT INCLUDES
#include "cedar/dynamics/fields/NeuralField.h"
#include "cedar/processing/Arguments.h"
#include "cedar/processing/LoopedTrigger.h"
#include "cedar/processing/Step.h"
#include "cedar/processing/StepTime.h"
#include "cedar/processing/Network.h"
#include "cedar/processing/ElementDeclaration.h"
#include "cedar/auxiliaries/sleepFunctions.h"
#include "cedar/auxiliaries/ObjectParameter.h"
#include "cedar/processing/DeclarationRegistry.h"
#include "cedar/processing/ExternalData.h"
#include "cedar/auxiliaries/kernel/Box.h"
#include "cedar/auxiliaries/casts.h"
#include "cedar/auxiliaries/ObjectListParameter.h"
#include "cedar/auxiliaries/ObjectParameter.h"

// SYSTEM INCLUDES
#include <iostream>

int main(int, char**)
{
  using cedar::proc::LoopedTrigger;
  using cedar::dyn::NeuralField;

  unsigned int errors = 0;

  std::cout << "Reading network.json ... " << std::endl;
  cedar::proc::NetworkPtr network(new cedar::proc::Network());
  network->readFile("network.json");
  std::cout << "done." << std::endl;

  cedar::aux::ConfigurablePtr convolution
    = network->getElement<NeuralField>("Field")->getConfigurableChild("lateral kernel convolution");
  cedar::aux::ObjectParameterPtr engine
    = cedar::aux::asserted_pointer_cast<cedar::aux::ObjectParameter>
      (
        convolution->getParameter("engine")
      );
  engine->setType("cedar.aux.conv.FFTW");

  cedar::aux::ObjectListParameterPtr kernel
    = cedar::aux::asserted_pointer_cast<cedar::aux::ObjectListParameter>
      (
        network->getElement<NeuralField>("Field")->getParameter("lateral kernels")
      );
  kernel->pushBack("cedar.aux.kernel.Box");

  // start the processing
  network->getElement<LoopedTrigger>("Main Trigger")->start();

  sleep(1);

  // stop the processing
  network->getElement<LoopedTrigger>("Main Trigger")->stop();

  // return
  std::cout << "Done. There were " << errors << " errors." << std::endl;
  return errors;
}