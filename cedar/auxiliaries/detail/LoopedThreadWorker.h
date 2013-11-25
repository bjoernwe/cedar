
/*=============================================================================

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

===============================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        LoopedThreadWorker.h

    Maintainer:  Jean-Stephane Jokeit
    Email:       jean-stephane.jokeit@ini.ruhr-uni-bochum.de
    Date:        Mon 07 Jan 2013 05:50:01 PM CET

    Description:

    Credits:

=============================================================================*/

#ifndef CEDAR_LOOPEDTHREADWORKER_H
#define CEDAR_LOOPEDTHREADWORKER_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/DoubleParameter.h"
#include "cedar/auxiliaries/BoolParameter.h"
#include "cedar/auxiliaries/EnumParameter.h"
#include "cedar/auxiliaries/LoopMode.h"
#include "cedar/auxiliaries/detail/ThreadWorker.h"

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/LoopedThread.fwd.h"
#include "cedar/auxiliaries/detail/LoopedThreadWorker.fwd.h"

// SYSTEM INCLUDES
#include <string>
#include <iostream>
#include <QThread>
#include <boost/date_time/posix_time/posix_time_types.hpp>

//! the corresponding worker class for the LoopedThread class
class cedar::aux::detail::LoopedThreadWorker : public cedar::aux::detail::ThreadWorker
{
  public: 
    //! constructor
    explicit LoopedThreadWorker(cedar::aux::LoopedThread* wrapper);
    //! destructor
    ~LoopedThreadWorker();

  private:
    //! initialize statistics at construction time
    void initStatistics();
    //! update statistics 
    inline void updateStatistics(double stepsTaken);
    //! request stop in the wrapper
    void safeRequestStop();
    //! check whether a stop has been requested
    bool safeStopRequested();

    //! set the step start time
    void setLastTimeStepStart(const boost::posix_time::ptime& time);
    //! set the step end time
    void setLastTimeStepEnd(const boost::posix_time::ptime& time);


  public:
    //! overwritten method that does the actual work
    void work();

  public:
    /*!@brief Returns the last timesteps start time.
     *
     */
    boost::posix_time::ptime getLastTimeStepStart() const;

    /*!@brief Returns the last timesteps end time.
     *
     */
    boost::posix_time::ptime getLastTimeStepEnd() const;

    /*!@brief Returns the last timesteps duration.
     *
     */
    boost::posix_time::time_duration getLastTimeStepDuration() const;

    //! return the current counter of steps
    unsigned long getNumberOfSteps();

    //!@todo: needs documentation. I don't get it. js
    double getMaxStepsTaken();

  private:
    //! we keep a pointer to the wrapper
    cedar::aux::LoopedThread* mpWrapper;

    //!@brief total number of steps since start()
    unsigned long mNumberOfSteps;
    //!@brief
    double mSumOfStepsTaken;
    //!@brief
    double mMaxStepsTaken;
    //!@brief remember time stamps of last step
    boost::posix_time::ptime mLastTimeStepStart;
    //!@brief remember time stamps of last step
    boost::posix_time::ptime mLastTimeStepEnd;

    //! lock for mNumberOfSteps
    mutable QReadWriteLock mNumberOfStepsLock;
    //! lock for mSumOfStepsTaken
    mutable QReadWriteLock mSumOfStepsTakenLock;
    //! lock for mMaxStepsTaken
    mutable QReadWriteLock mMaxStepsTakenLock;
    //! lock for mLastTimeStepStart
    mutable QReadWriteLock mLastTimeStepStartLock;
    //! lock for mLastTimeStepEnd
    mutable QReadWriteLock mLastTimeStepEndLock;
};


#endif

