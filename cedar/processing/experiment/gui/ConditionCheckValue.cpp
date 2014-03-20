/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        ConditionCheckValue.cpp

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.rub.de
    Date:        2014 02 26

    Description: Source file for the class cedar::proc::experiment::gui::ConditionCheckValue.

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/experiment/gui/ConditionCheckValue.h"
#include "cedar/processing/experiment/ExperimentController.h"
#include "cedar/processing/experiment/Experiment.h"
#include "cedar/auxiliaries/gui/Parameter.h"

// SYSTEM INCLUDES
#include "cedar/processing/experiment/ConditionCheckValue.h"
#include <QLabel>

//----------------------------------------------------------------------------------------------------------------------
// associate experiment::gui::Condition with the experiment::Condition
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered = cedar::proc::experiment::gui::ConditionFactorySingleton::getInstance()->add
      <
        cedar::proc::experiment::ConditionCheckValue,
        cedar::proc::experiment::gui::ConditionCheckValue
      >();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::experiment::gui::ConditionCheckValue::ConditionCheckValue()
{

}

cedar::proc::experiment::gui::ConditionCheckValue::~ConditionCheckValue()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::experiment::gui::ConditionCheckValue::redraw()
{
  if(mCondition)
  {

    cedar::aux::ParameterPtr step_para = this->mCondition->getParameter("StepProperty");
    mStepSelector = cedar::aux::gui::ParameterFactorySingleton::getInstance()->get(step_para)->allocateRaw();
    mStepSelector->setParameter(step_para);
    mStepSelector->setParent(this);
    mLayout->addWidget(mStepSelector);

    //get desired Value
    cedar::aux::ParameterPtr desiredValue = mCondition->getParameter("DesiredValue");
    cedar::aux::gui::Parameter* desiredValueWidget =cedar::aux::gui::ParameterFactorySingleton::getInstance()->get(desiredValue)->allocateRaw();
    desiredValueWidget->setParameter(desiredValue);
    desiredValueWidget->setParent(this);
    mLayout->addWidget(desiredValueWidget);
  }
}

