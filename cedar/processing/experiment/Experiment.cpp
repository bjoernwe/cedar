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

    File:        Experiment.cpp

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2014 01 22

    Description:

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/processing/experiment/Experiment.h"
#include "cedar/auxiliaries/GlobalClock.h"
#include "cedar/auxiliaries/Recorder.h"
#include "cedar/processing/experiment/action/StartAllTriggers.h"
#include "cedar/processing/experiment/action/StartTrigger.h"
#include "cedar/processing/experiment/condition/OnInit.h"
#include "cedar/processing/experiment/ExperimentSuperviser.h"
#include "cedar/processing/Step.h"
#include "cedar/processing/Trigger.h"
#include "cedar/processing/LoopedTrigger.h"
#include "cedar/auxiliaries/ParameterDeclaration.h"
#include "cedar/auxiliaries/sleepFunctions.h"

// SYSTEM INCLUDES
#include <boost/bind.hpp>
#ifndef Q_MOC_RUN
  #include <boost/signals2/signal.hpp>
  #include <boost/signals2/connection.hpp>
#endif

//----------------------------------------------------------------------------------------------------------------------
// static members
//----------------------------------------------------------------------------------------------------------------------
cedar::aux::EnumType<cedar::proc::experiment::Experiment::ResetType>
    cedar::proc::experiment::Experiment::ResetType::mType("Expermient.ResetType.");

cedar::aux::EnumType<cedar::proc::experiment::Experiment::CompareMethod>
    cedar::proc::experiment::Experiment::CompareMethod::mType("Expermient.CompareMethod.");

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::proc::experiment::Experiment::Experiment(cedar::proc::GroupPtr group)
:
mGroup(group),
mActualTrial(0),
mInit(false),
mStopped(true),
_mFileName(new cedar::aux::StringParameter(this, "filename", "")),
_mTrials(new cedar::aux::UIntParameter(this, "repetitions", 1)),
_mActionSequences
(
  new ActionSequencelListParameter
  (
    this,
    "ActionSequences",
    std::vector<ActionSequencePtr>()
  )
)
{
  ExperimentSuperviserSingleton::getInstance()->setExperiment(this);

  // Create first action sequence
  ActionSequencePtr as = ActionSequencePtr(new ActionSequence());
  as->addAction(cedar::proc::experiment::action::ActionPtr(new cedar::proc::experiment::action::StartAllTriggers()));
  as->setName("ActionSequence1");
  this->addActionSequence(as);

  // Initialize Group starter and stopper
  this->mStartGroup = cedar::aux::CallFunctionInThreadPtr
                              (
                                new cedar::aux::CallFunctionInThread
                                (
                                  boost::bind(&cedar::proc::Group::startTriggers, group, true)
                                )
                              );

  this->mStopGroup  = cedar::aux::CallFunctionInThreadPtr
                             (
                               new cedar::aux::CallFunctionInThread
                               (
                                 boost::bind(&cedar::proc::Group::stopTriggers, group, true)
                               )
                             );

  mElementRemovedConnection = this->mGroup->connectToElementRemovedSignal
  (
    boost::bind(&cedar::proc::experiment::Experiment::groupChanged,this,_1)
  );
  mNewElementAddedConnection = this->mGroup->connectToNewElementAddedSignal
  (
    boost::bind(&cedar::proc::experiment::Experiment::groupChanged,this,_1)
  );
}
cedar::proc::experiment::Experiment::~Experiment()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------
void cedar::proc::experiment::Experiment::groupChanged(cedar::proc::ConstElementPtr element)
{
  emit groupChanged();
}

const std::string& cedar::proc::experiment::Experiment::getFileName() const
{
  return _mFileName->getValue();
}

void cedar::proc::experiment::Experiment::setFileName(const std::string& filename)
{
  _mFileName->setValue(filename);
}

unsigned int cedar::proc::experiment::Experiment::getTrialCount() const
{
  return _mTrials->getValue();
}

void cedar::proc::experiment::Experiment::setTrialCount(unsigned int repetitions)
{
  _mTrials->setValue(repetitions);
}

void cedar::proc::experiment::Experiment::run()
{
  if (this->_mTrials->getValue() > 0 && checkActionSequences())
  {
    // Set record directory
    std::string time_stamp = cedar::aux::RecorderSingleton::getInstance()->getTimeStamp();
    this->mRecordFolderName = this->_mName->getValue()+ "_" + time_stamp;
    this->mActualTrial = 1;
    ExperimentSuperviserSingleton::getInstance()->start();
    emit experimentRunning(true);
  }
}
void cedar::proc::experiment::Experiment::cancel()
{
  ExperimentSuperviserSingleton::getInstance()->requestStop();
  stopTrial();
  emit experimentRunning(false);
}

void cedar::proc::experiment::Experiment::startTrial()
{
  mStopped=false;
  emit trialNumberChanged(mActualTrial);
  cedar::aux::GlobalClockSingleton::getInstance()->start();

  //start records
  std::stringstream ss;
  ss << mActualTrial;
  std::string trial_number = ss.str();
  cedar::aux::RecorderSingleton::getInstance()->setSubfolder(mRecordFolderName+"/"+"Trial_"+trial_number+"_#T#");
  cedar::aux::RecorderSingleton::getInstance()->start();
}


void cedar::proc::experiment::Experiment::startTrigger(const std::string& triggerName)
{
  if(mStopped)
  {
    startTrial();
  }
  for (auto name_element_pair : this->mGroup->getElements())
    {
      if (cedar::proc::TriggerPtr trigger = boost::dynamic_pointer_cast<cedar::proc::Trigger>(name_element_pair.second))
      {
        if(name_element_pair.first == triggerName)
        {
          if (auto looped_trigger = boost::dynamic_pointer_cast<cedar::proc::LoopedTrigger>(trigger))
          {
            looped_trigger->start();
          }
        }
      }
    }
}


void cedar::proc::experiment::Experiment::startAllTriggers()
{
  if(mStopped)
  {
    startTrial();
  }
  this->mStartGroup->start();
}

void cedar::proc::experiment::Experiment::addActionSequence(cedar::proc::experiment::ActionSequencePtr actionSequence)
{
  this->_mActionSequences->pushBack(actionSequence);
}

std::vector<cedar::proc::experiment::ActionSequencePtr> cedar::proc::experiment::Experiment::getActionSequences()
{
  std::vector<cedar::proc::experiment::ActionSequencePtr> ret;
  for (unsigned int i = 0; i < _mActionSequences->size(); i++)
  {
    ret.push_back(this->_mActionSequences->at(i));
  }
  return ret;
}

void cedar::proc::experiment::Experiment::stopTrial(ResetType::Id reset)
{
  this->mStopGroup->start();
  cedar::aux::RecorderSingleton::getInstance()->stop();
  cedar::aux::GlobalClockSingleton::getInstance()->stop();
  cedar::aux::GlobalClockSingleton::getInstance()->reset();

  // Apply the different reset types
  switch(reset)
  {
    case ResetType::None:
    {
      break;
    }
    case ResetType::Wait:
    {
      cedar::aux::usleep(1000000);
      break;
    }
    case ResetType::Reset:
    {
      this->mGroup->reset();
      break;
    }
    case ResetType::Reload:
    {
      //@todo Reload function
      break;
    }
    default:
    {
      this->mGroup->reset();
      break;
    }
  }
  mActualTrial++;

  // Stop the experiment if the actual trial exceeds the number of wanted trials
  if ( mActualTrial >_mTrials->getValue() )
  {
    ExperimentSuperviserSingleton::getInstance()->requestStop();
    emit experimentRunning(false);
    mActualTrial  = 0;
  }
  mStopped=true;
  emit trialNumberChanged(mActualTrial);
}

void cedar::proc::experiment::Experiment::executeAcionSequences(bool initial)
{
  this->mInit = initial;
  for (ActionSequencePtr action_sequence: this->getActionSequences())
  {
    if(action_sequence->getCondition()->check())
    {
      for(cedar::proc::experiment::action::ActionPtr action : action_sequence->getActions())
      {
        action->run();
      }
    }
  }
  this->mInit = false;
}

void cedar::proc::experiment::Experiment::removeActionSequence(
    cedar::proc::experiment::ActionSequencePtr actionSequence)
{
  for (unsigned int i = 0; i < _mActionSequences->size(); i++)
  {
    if (this->_mActionSequences->at(i) == actionSequence)
    {
      this->_mActionSequences->removeObject(i);
      return;
    }
  }
}

bool cedar::proc::experiment::Experiment::isOnInit()
{
  return mInit;
}

std::vector<std::string> cedar::proc::experiment::Experiment::getGroupSteps()
{
  std::vector<std::string> ret;
  for (auto name_element_pair : this->mGroup->getElements())
  {
    if (cedar::proc::StepPtr step = boost::dynamic_pointer_cast<cedar::proc::Step>(name_element_pair.second))
    {
      ret.push_back(name_element_pair.first);
    }
  }
  return ret;
}

std::vector<std::string> cedar::proc::experiment::Experiment::getGroupTriggers()
{
  std::vector<std::string> ret;
  for (auto name_element_pair : this->mGroup->getElements())
  {
    if (cedar::proc::TriggerPtr trigger = boost::dynamic_pointer_cast<cedar::proc::Trigger>(name_element_pair.second))
    {
      ret.push_back(name_element_pair.first);
    }
  }
  return ret;
}

std::vector<std::string> cedar::proc::experiment::Experiment::getStepParameters(std::string step, const std::vector<std::string>& allowedTypes)
{
  cedar::proc::StepPtr stepItem =this->mGroup->getElement<cedar::proc::Step>(step);

  std::vector<std::string> ret;
  for (cedar::aux::ParameterPtr parameter :  stepItem->getParameters())
  {
    try
    {
      // Check if parameter is registered in the DeclarationManager
      std::string parameter_type = cedar::aux::ParameterDeclarationManagerSingleton::getInstance()->getTypeId(parameter);
      if(allowedTypes.size() > 0)
      {
        for(std::string type : allowedTypes)
        {
          if (type == parameter_type)
          {
            ret.push_back(parameter->getName());
            break;
          }
        }
      }
      else
      {
        ret.push_back(parameter->getName());
      }
    }
    catch(cedar::aux::UnknownTypeException e)
    {
       //@todo
    }
  }
  return ret;
}

cedar::aux::ParameterPtr cedar::proc::experiment::Experiment::getStepParameter(std::string step, std::string parameter)
{
  cedar::proc::StepPtr stepItem =this->mGroup->getElement<cedar::proc::Step>(step);

  return stepItem->getParameter(parameter);
}

std::vector<std::string> cedar::proc::experiment::Experiment::getStepDatas(std::string step, cedar::proc::DataRole::Id role )
{
  cedar::proc::StepPtr stepItem =this->mGroup->getElement<cedar::proc::Step>(step);

  std::vector<std::string> ret;
  for (auto data :  stepItem->getDataSlots(role))
  {
    ret.push_back(data.first);
  }
  return ret;
}

cedar::aux::ConstDataPtr cedar::proc::experiment::Experiment::getStepData(std::string step, std::string value,cedar::proc::DataRole::Id role)
{
  cedar::proc::StepPtr stepItem =this->mGroup->getElement<cedar::proc::Step>(step);
  return stepItem->getData(role,value);
}

unsigned int cedar::proc::experiment::Experiment::getActualTrial()
{
  return this->mActualTrial;
}
bool cedar::proc::experiment::Experiment::hasStopped()
{
  return mStopped;
}
bool cedar::proc::experiment::Experiment::checkActionSequences()
{
  for (ActionSequencePtr action_sequence: this->getActionSequences())
  {
    if(boost::dynamic_pointer_cast<cedar::proc::experiment::condition::OnInit>(action_sequence->getCondition()))
    {
      for(cedar::proc::experiment::action::ActionPtr action : action_sequence->getActions())
      {
        if(boost::dynamic_pointer_cast<cedar::proc::experiment::action::StartAllTriggers>(action))
        {
          return true;
        }

        if(boost::dynamic_pointer_cast<cedar::proc::experiment::action::StartTrigger>(action))
        {
          return true;
        }
      }
    }
  }
  return false;
}
