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

    File:        triggerChains.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 08 08

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES

// CEDAR INCLUDES
#include "cedar/processing/Network.h"
#include "cedar/processing/Step.h"
#include "cedar/processing/Trigger.h"
#include "cedar/auxiliaries/DataTemplate.h"
#include "cedar/auxiliaries/CallFunctionInThread.h"
#include "cedar/auxiliaries/stringFunctions.h"
#include "cedar/auxiliaries/Log.h"

// SYSTEM INCLUDES
#include <QCoreApplication>
#include <boost/make_shared.hpp>
#include <limits.h>


// ---------------------------------------------------------------------------------------------------------------------

typedef cedar::aux::DataTemplate<unsigned int> UIntData;
CEDAR_GENERATE_POINTER_TYPES(UIntData);

class TriggerTest : public cedar::proc::Step
{
public:
  TriggerTest()
  :
  mTriggerCount(0),
  mDataOut(new UIntData(0))
  {
    this->declareInput("in1", false);
    this->declareInput("in2", false);
    this->declareOutput("out", mDataOut);
  }

  void inputConnectionChanged(const std::string& inputName)
  {
    auto data = boost::dynamic_pointer_cast<ConstUIntData>(this->getInput(inputName));
    if (inputName == "in1")
    {
      this->mDataIn1 = data;
    }
    else if (inputName == "in2")
    {
      this->mDataIn2 = data;
    }
  }

  void compute(const cedar::proc::Arguments&)
  {
    ++mTriggerCount;

    // copy current value of the input

    unsigned int data1 = std::numeric_limits<unsigned int>::max();
    if (this->mDataIn1)
    {
      data1 = this->mDataIn1->getData();
    }
    unsigned int data2 = std::numeric_limits<unsigned int>::max();
    if (this->mDataIn2)
    {
      data2 = this->mDataIn2->getData();
    }

    // we always use the "oldest" value there is, but only if inputs are connected
    if (this->mDataIn1 || this->mDataIn2)
    {
      this->mDataOut->setData(std::min(data1, data2));
    }

    std::cout << this->getName() << " triggered; data1 = ";
    if (data1 < std::numeric_limits<unsigned int>::max())
    {
      std::cout << data1;
    }
    else
    {
      std::cout << "n/a";
    }
    std::cout << ", data2 = ";
    if (data2 < std::numeric_limits<unsigned int>::max())
    {
      std::cout << data2;
    }
    else
    {
      std::cout << "n/a";
    }
    std::cout << ", data_out = " << this->mDataOut->getData() << std::endl;
  }

  void resetData()
  {
    this->mDataOut->setData(0);
    this->mTriggerCount = 0;
  }

  unsigned int mTriggerCount;

  // these data are a kind of time stamp; i.e., the higher the value in them, the more recent they are.
  ConstUIntDataPtr mDataIn1;
  ConstUIntDataPtr mDataIn2;
  UIntDataPtr mDataOut;
};

CEDAR_GENERATE_POINTER_TYPES(TriggerTest);

int global_errors = 0;
int num_superfluous_triggers = 0;

void insert_triggered_children(TriggerTestPtr step, std::set<TriggerTestPtr>& triggered)
{
  cedar::proc::TriggerPtr done_trigger = step->getFinishedTrigger();
  for (size_t i = 0; i < done_trigger->getListeners().size(); ++i)
  {
    if (auto listener = boost::dynamic_pointer_cast<TriggerTest>(done_trigger->getListeners().at(i)))
    {
      if (triggered.find(listener) == triggered.end())
      {
        triggered.insert(listener);
        insert_triggered_children(listener, triggered);
      }
    }
  }
}

void test_step(cedar::proc::NetworkPtr network, TriggerTestPtr step)
{
  // build a list of all steps triggered by the one being checked
  std::set<TriggerTestPtr> triggered;
  insert_triggered_children(step, triggered);

  std::cout << std::endl;
  std::cout << "Steps to check when triggering \"" << step->getName() << "\" are: ";
  for (auto iter = triggered.begin(); iter != triggered.end(); ++iter)
  {
    if (iter != triggered.begin())
    {
      std::cout << ", ";
    }
    std::cout << (*iter)->getName();
  }
  std::cout << std::endl;

  // reset data of all steps
  std::cout << "Resetting data." << std::endl;
  for (auto iter = network->getElements().begin(); iter != network->getElements().end(); ++iter)
  {
    cedar::proc::ElementPtr element = iter->second;
    if (auto trigger_test = boost::dynamic_pointer_cast<TriggerTest>(element))
    {
      // reset data (marks it as out-of-date)
      trigger_test->resetData();
      if (triggered.find(trigger_test) == triggered.end())
      {
        // mark the step as up-to-date if it is not triggered
        trigger_test->mDataOut->setData(1);
      }
    }
  }
  // also, mark the step under investigation as up-to-date
  step->mDataOut->setData(1);

  // trigger step; this should propagate all of its data to the subsequent steps
  std::cout << "Triggering step." << std::endl;
  step->onTrigger();

  // check all triggered steps for being up-to-date (they should be after being triggered.

  for (auto iter = network->getElements().begin(); iter != network->getElements().end(); ++iter)
  {
    cedar::proc::ElementPtr element = iter->second;
    if (auto trigger_test = boost::dynamic_pointer_cast<TriggerTest>(element))
    {
      bool is_triggered = triggered.find(trigger_test) != triggered.end();

      auto value = trigger_test->mDataOut->getData();
      if (is_triggered && value != 1)
      {
        cedar::aux::LogSingleton::getInstance()->error
        (
          "Data of step \"" + trigger_test->getName()
            + "\" is not up-to-date (is " + cedar::aux::toString(value) + " instead of 1)",
          ""
        );
        ++global_errors;
      }

      auto trigger_count = trigger_test->mTriggerCount;
      if (is_triggered)
      {
        if (trigger_count > 1)
        {
          cedar::aux::LogSingleton::getInstance()->warning
          (
            "Step \"" + trigger_test->getName() + "\" was triggered more than once ("
              + cedar::aux::toString(trigger_count) + " times)",
            ""
          );
          ++num_superfluous_triggers;
        }

        if (trigger_count < 1)
        {
          cedar::aux::LogSingleton::getInstance()->error
          (
            "Step \"" + trigger_test->getName() + "\" was not triggered enough ("
              + cedar::aux::toString(trigger_count) + " times instead of 1 time)",
            ""
          );
          ++global_errors;
        }
      }
      else if (step != trigger_test)
      {
        if (trigger_count > 0)
        {
          cedar::aux::LogSingleton::getInstance()->error
          (
            "Step \"" + trigger_test->getName() + "\" was triggered even though it should not have been ("
              + cedar::aux::toString(trigger_count) + " times)",
            ""
          );
          ++global_errors;
        }
      }
    }
  }
}

void test_network(cedar::proc::NetworkPtr network)
{
  using cedar::proc::Network;
  using cedar::proc::NetworkPtr;

  for (auto iter = network->getElements().begin(); iter != network->getElements().end(); ++iter)
  {
    cedar::proc::ElementPtr element = iter->second;
    if (auto step = boost::dynamic_pointer_cast<TriggerTest>(element))
    {
      test_step(network, step);
    }
  }
}


void run_test()
{
  using cedar::proc::Network;
  using cedar::proc::NetworkPtr;

  {
    std::cout << "==================================" << std::endl;
    std::cout << " Checking network configuration 1" << std::endl;
    std::cout << "==================================" << std::endl << std::endl;

    NetworkPtr network(new Network());
    network->add(boost::make_shared<TriggerTest>(), "step1");
    network->add(boost::make_shared<TriggerTest>(), "step2");
    network->add(boost::make_shared<TriggerTest>(), "step3");
    network->add(boost::make_shared<TriggerTest>(), "step4");

    std::cout << "Connecting step1.out -> step2.in1" << std::endl;
    network->connectSlots("step1.out", "step2.in1");
    std::cout << "Connecting step1.out -> step3.in1" << std::endl;
    network->connectSlots("step1.out", "step3.in1");
    std::cout << "Connecting step2.out -> step4.in1" << std::endl;
    network->connectSlots("step2.out", "step4.in1");
    std::cout << "Connecting step3.out -> step4.in1" << std::endl;
    network->connectSlots("step3.out", "step4.in2");

    test_network(network);
  }
}

int main(int argc, char** argv)
{
  QCoreApplication* app;
  app = new QCoreApplication(argc,argv);

  auto testThread = new cedar::aux::CallFunctionInThread(run_test);

  QObject::connect(testThread, SIGNAL(finishedThread()), app, SLOT(quit()), Qt::QueuedConnection);  // alternatively: call app->quit() in runTests()

  testThread->start();
  app->exec();

  delete testThread;
  delete app;

  std::cout << "Superfluous trigger calls: " << num_superfluous_triggers << std::endl;
  std::cout << "Test finished with " << global_errors << " error(s)." << std::endl;

  return global_errors;
}

