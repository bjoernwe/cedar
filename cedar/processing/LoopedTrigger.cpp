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

    File:        LoopedTrigger.cpp


    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 06

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/processing/LoopedTrigger.h"
#include "cedar/processing/StepTime.h"
#include "cedar/units/TimeUnit.h"
#include "cedar/processing/Manager.h"
#include "cedar/processing/LoopMode.h"
#include "cedar/auxiliaries/System.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <algorithm>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::LoopedTrigger::LoopedTrigger(double stepSize)
:
cedar::aux::LoopedThread(stepSize),
//!@todo Should these parameters go into cedar::aux::LoopedThread?
mLoopType(new cedar::aux::EnumParameter(
                                         this,
                                         "LoopMode",
                                         cedar::proc::LoopMode::typePtr(),
                                         cedar::proc::LoopMode::FIXED_ADAPTIVE
                                       )
         ),
//!@todo Make a TimeParameter and use it here instead.
mLoopTime(new cedar::aux::DoubleParameter(this, "LoopTime", 1.0, 1.0, 1000000.0))
{

  QObject::connect(this->mLoopType.get(), SIGNAL(valueChanged()), this, SLOT(loopModeChanged()));
  QObject::connect(this->mLoopTime.get(), SIGNAL(valueChanged()), this, SLOT(loopTimeChanged()));
}

cedar::proc::LoopedTrigger::~LoopedTrigger()
{
  this->stopTrigger();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::LoopedTrigger::loopModeChanged()
{
  this->mLoopTime->setConstant(false);
  this->useFixedStepSize(false);
  switch (this->mLoopType->getValue())
  {
    case cedar::proc::LoopMode::FIXED:
      this->setStepSize(this->mLoopTime->getValue());
    default:
    case cedar::proc::LoopMode::FIXED_ADAPTIVE:
      this->useFixedStepSize(true);
      break;

    case cedar::proc::LoopMode::REALTIME:
      this->mLoopTime->setConstant(true);
      this->setSimulatedTime();
      this->setStepSize(0.0);
      break;
  }
}

void cedar::proc::LoopedTrigger::loopTimeChanged()
{
  this->setStepSize(this->mLoopTime->getValue());
}

void cedar::proc::LoopedTrigger::removeListener(cedar::proc::StepPtr step)
{
  this->cedar::proc::Trigger::removeListener(step);
  //!@todo This assumes that the step is only connected to one looped trigger. However, this constraint is not expressed in the framework, yet.
  step->onStop();
}

void cedar::proc::LoopedTrigger::startTrigger()
{
  for (size_t i = 0; i < this->mListeners.size(); ++i)
  {
    this->mListeners.at(i)->onStart();
  }
  CEDAR_NON_CRITICAL_ASSERT(!this->isRunning());
  this->start();
}

void cedar::proc::LoopedTrigger::stopTrigger()
{
  this->stop(2000);

  for (size_t i = 0; i < this->mListeners.size(); ++i)
  {
    this->mListeners.at(i)->onStop();
  }
}

void cedar::proc::LoopedTrigger::step(double time)
{
  cedar::proc::ArgumentsPtr arguments (new cedar::proc::StepTime(cedar::unit::Milliseconds(time)));

  this->trigger(arguments);
  usleep(100);
}
