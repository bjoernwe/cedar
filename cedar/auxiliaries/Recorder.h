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

    File:        Recorder.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2013 07 01

    Description: Singleton class for recording and saving the data generated in a network.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_RECORDER_H_
#define CEDAR_AUX_RECORDER_H_

// CEDAR INCLUDES
#include "cedar/processing/namespace.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/auxiliaries/LoopedThread.h"
#include "cedar/auxiliaries/ThreadCollection.h"
#include "DataSpectator.h"


// SYSTEM INCLUDES
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <QReadWriteLock>
#include <QTime>
#include <fstream>




  /*!@brief Singleton class for recording and saving the data registered to disk.
   */
class cedar::aux::Recorder : public cedar::aux::LoopedThread
{

  //--------------------------------------------------------------------------------------------------------------------
  // friends
  //--------------------------------------------------------------------------------------------------------------------

  // uses singleton template.
  friend class cedar::aux::Singleton<Recorder>;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief The private constructor for singleton usage.
  Recorder();
public:
  //!@brief The Destructor.
  ~Recorder();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Can be called to register a DataPtr to be recorded.
   *
   *             It will create a new thread which will record the data in the specified interval.
   *             Recording will only start when cedar::aux::Recorder::start() is called. If name is already
   *             exist in the Recorder, the DataPtr will not be registered.
   *
   */
  void registerData(cedar::aux::ConstDataPtr toSpectate, unsigned  int recordIntv, const std::string& name);

  //!@brief Used to unregister a DataPtr to stop him from beeing recorded.
  void unregisterData(const std::string& name);

  //!@brief Unregister all DataPtr.
  void clear();


  //!@brief Sets the directory the recorded data will be written to.
  void setOutputDirectory(const std::string& path);

  //!@brief Gets the OutputDirectory
  std::string getOutputDirectory();

  //!@brief Returns the elapsed time in ms since the REcorder thread has been started.
  int getTimeStamp();

  /*!@brief Change the record interval of 'name'
   *          If 'name' is not a registered it will throw an UnknownNameExeption.
   */
  void setRecordIntervalTime(const std::string& name, unsigned int recordIntv);

  /*!@brief Returns the specified record interval of the DataPtr 'name' in ms.
   *         It will return -1 if name was not registered
   */
  int getRecordIntervalTime(const std::string& name);

  //!@brief Checks if a DataPtr with a certain name is registered.
  bool isRegistered(const std::string& name);
  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  /*!@brief Starts the recorder thread.
  *
  *              By calling start, every registered observer thread will automatically be started.
  *              Additionally all files will be created and filled with headers.
  */
  void applyStart();

  /*!@brief Stops the recorder thread.
   *
   *              By calling stop, the calling thread waits until all recorded data has been written to disk.
   */
  void applyStop(bool suppressWarning);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief Calls write in the specified interval.
  void step(double time);

  //!@brief Creates a new Output directory
  void createOutputDirectory();

//--------------------------------------------------------------------------------------------------------------------
// members
//--------------------------------------------------------------------------------------------------------------------

protected:
  // none yet

private:
  //!@brief The registered DataSpectaors.
  cedar::aux::ThreadCollection mDataSpectatorCollection;

  //!@brief The output directory.
  std::string mOutputDirectory;

  //!@brief Counts the time from when the recorder is started.
  QTime mStartTime;
};



#include "cedar/auxiliaries/Singleton.h"

namespace cedar
{
  namespace aux
    {
      CEDAR_INSTANTIATE_PROC_TEMPLATE(cedar::aux::Singleton<Recorder>);
      typedef cedar::aux::Singleton<Recorder> RecorderSingleton;
    }
}

#endif // CEDAR_AUX_RECORDER_H
