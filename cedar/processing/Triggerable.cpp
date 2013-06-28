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

    File:        Triggerable.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 11 16

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/Triggerable.h"
#include "cedar/processing/Trigger.h"
#include "cedar/auxiliaries/assert.h"

// SYSTEM INCLUDES
#include <QReadLocker>
#include <QWriteLocker>
#include <QMutexLocker>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------
cedar::proc::Triggerable::Triggerable(bool isLooped)
:
mIsLooped(isLooped),
mState(cedar::proc::Triggerable::STATE_UNKNOWN),
mStartCalls(0),
mpStartCallsLock(new QMutex())
{
}

cedar::proc::Triggerable::~Triggerable()
{
  delete this->mpStartCallsLock;
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

boost::signals2::connection cedar::proc::Triggerable::connectToStateChanged(boost::function<void ()> slot)
{
  return mStateChanged.connect(slot);
}

void cedar::proc::Triggerable::setParentTrigger(cedar::proc::TriggerPtr parent)
{
  if (this->isLooped())
  {
    // If there is already a parent trigger for looped steps, disconnect it first!
    CEDAR_ASSERT(!parent || !this->mParentTrigger.lock());
  }
  this->mParentTrigger = parent;
}

cedar::proc::TriggerPtr cedar::proc::Triggerable::getParentTrigger()
{
  return this->mParentTrigger.lock();
}

void cedar::proc::Triggerable::callOnStart()
{
  // make sure no other thread can start the step
  QMutexLocker locker(this->mpStartCallsLock);

  //!@todo onStart might take a long time - can/should this be done better, e.g., by first storing a bool, then incrementing, then unlocking, then calling onStart?
  // only call onStart if this triggerable hasn't been started yet
  if (this->mStartCalls == 0)
  {
    this->onStart();
  }

  // count how often this triggerable was started
  ++this->mStartCalls;

  locker.unlock();

  if (mFinished)
  {
    for (size_t i = 0; i < this->mFinished->getListeners().size(); ++i)
    {
      this->mFinished->getListeners().at(i)->callOnStart();
    }
  }
}

void cedar::proc::Triggerable::callOnStop()
{
  // only call onStop if there is only one trigger left that started this triggerable
  QMutexLocker locker(this->mpStartCallsLock);

  if (this->mStartCalls == 1)
  {
    this->onStop();
  }

  // count how often this was stopped
  CEDAR_ASSERT(this->mStartCalls > 0);
  --this->mStartCalls;
  locker.unlock();

  this->setState(cedar::proc::Triggerable::STATE_UNKNOWN, "");

  if (mFinished)
  {
    for (size_t i = 0; i < this->mFinished->getListeners().size(); ++i)
    {
      this->mFinished->getListeners().at(i)->callOnStop();
    }
  }
}

void cedar::proc::Triggerable::setState(cedar::proc::Triggerable::State newState, const std::string& annotation)
{
  // Only act if the state actually changes.
  QWriteLocker locker(&mStateLock);
  if (newState != this->mState || annotation != this->mStateAnnotation)
  {
    this->mState = newState;
    this->mStateAnnotation = annotation;
    locker.unlock();
    mStateChanged();
//    emit stateChanged();
  }
}

cedar::proc::Triggerable::State cedar::proc::Triggerable::getState() const
{
  QReadLocker locker(&mStateLock);
  return this->mState;
}

std::string cedar::proc::Triggerable::getStateAnnotation() const
{
  QReadLocker locker(&mStateLock);
  return this->mStateAnnotation;
}

void cedar::proc::Triggerable::onStart()
{
  // empty as a default implementation
}

void cedar::proc::Triggerable::onStop()
{
  // empty as a default implementation
}

cedar::proc::TriggerPtr cedar::proc::Triggerable::getFinishedTrigger()
{
  if (!this->mFinished)
  {
    mFinished = cedar::proc::TriggerPtr(new cedar::proc::Trigger("processingDone"));
  }
  return this->mFinished;
}
