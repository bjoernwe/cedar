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

    File:        Grabber.cpp

    Maintainer:  Georg.Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 08 01

    Description: Implementation of the @em cedar::Grabber class.

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/devices/sensors/visual/Grabber.h"

// SYSTEM INCLUDES
#include <signal.h>

//----------------------------------------------------------------------------------------------------------------------
//constants and static variables
//----------------------------------------------------------------------------------------------------------------------

const std::string cedar::dev::sensors::visual::Grabber::mGrabberDefaultRecordExtension =  ".avi";
const std::string cedar::dev::sensors::visual::Grabber::mGrabberDefaultSnapshotExtension = ".jpg";

//Initialize the static member vector containing all instances
cedar::dev::sensors::visual::Grabber::GrabberInstancesVector cedar::dev::sensors::visual::Grabber::mInstances;
bool cedar::dev::sensors::visual::Grabber::mRegisterTerminationHandler;

//----------------------------------------------------------------------------------------------------------------------
//constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------
cedar::dev::sensors::visual::Grabber::Grabber(const std::string&)
:
cedar::aux::LoopedThread(),
_mName(new cedar::aux::StringParameter(this, "name", ""))
{
  cedar::aux::LogSingleton::getInstance()->allocating(this);

  //initialize local member
  mpReadWriteLock = new QReadWriteLock();
  mRecord = false;
  mGrabberThreadStartedOnRecording = false;
  mCleanUpAlreadyDone = false;
  mFpsMeasureStart = boost::posix_time::microsec_clock::local_time();
  mFpsMeasureStop = boost::posix_time::microsec_clock::local_time();
  mFpsCounter = 0;
  mFpsMeasured = 0;
}

//--------------------------------------------------------------------------------------------------------------------
cedar::dev::sensors::visual::Grabber::~Grabber()
{
  //remove this grabber-instance from the InstancesVector
  std::vector<Grabber *>::iterator it = mInstances.begin();

  while (((*it) != this) && (it != mInstances.end()))
  {
    ++it;
  }

  if ((*it) == this)
  {
    cedar::aux::LogSingleton::getInstance()->debugMessage
                                            (
                                              this->getName() + ": This grabber "
                                                + "delete from list of all instances.",
                                              "cedar::dev::sensors::visual::GrabberInterface::~GrabberInterface()"
                                            );
    mInstances.erase(it);
  }
  cedar::aux::LogSingleton::getInstance()->freeing(this);
}

//----------------------------------------------------------------------------------------------------------------------
//methods for signal handling and cleanup
//----------------------------------------------------------------------------------------------------------------------

//this function handles abnormal termination of the program (like the signals interrupt, abort or seg.fault)
void cedar::dev::sensors::visual::Grabber::interruptSignalHandler(int)
{
  emergencyCleanup();
  std::exit(1);
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::emergencyCleanup()
{
  cedar::aux::LogSingleton::getInstance()->message
                                           (
                                             "Emergency Cleanup",
                                             "cedar::dev::sensors::visual::Grabber::emergencyCleanup()"
                                           );

  for (std::vector<Grabber*>::iterator it = mInstances.begin(); it != mInstances.end(); ++it)
  {
    //only cedar::dev::sensors::visual::Grabber::doCleanUp() and xxxGrabber::onCleanUp() methods invoked
    //Note: no destructor involved here
    (*it)->doCleanUp();
  }
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::installCrashHandler()
{
   signal(SIGINT,&Grabber::interruptSignalHandler);
   signal(SIGABRT,&Grabber::interruptSignalHandler);
   signal(SIGSEGV,&Grabber::interruptSignalHandler);

   cedar::aux::LogSingleton::getInstance()->message
                                            (
                                              "Crash handler installed",
                                              "cedar::dev::sensors::visual::GrabberInterface::installCrashHandler()"
                                            );
}


//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::doCleanUp()
{
  if (! mCleanUpAlreadyDone)
  {

    cedar::aux::LogSingleton::getInstance()->debugMessage
                                             (
                                              this->getName() + ": cleanup",
                                               "cedar::dev::sensors::visual::GrabberInterface::doCleanUp()"
                                             );

    mCleanUpAlreadyDone = true;

    //stopGrabber LoopedThread
    if (QThread::isRunning())
    {
      cedar::aux::LoopedThread::stop();
      sleep(500);
    }

    //stopGrabber Recording
    stopRecording();

    //do the cleanup in derived class
    onCleanUp();
    //!todo: check if pointer need to call the delete-function
    mChannels.clear();

    if (mpReadWriteLock)
    {
      delete mpReadWriteLock;
      mpReadWriteLock = NULL;
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------
// configurable interface
//--------------------------------------------------------------------------------------------------------------------

/*
void cedar::dev::sensors::visual::Grabber::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
cedar::aux::NamedConfigurable

  //do readConfiguration for all childs. This is implemented in the base class
  cedar::aux::NamedConfigurable::readConfiguration(node);

}
*/

//--------------------------------------------------------------------------------------------------------------------
// class methods
//--------------------------------------------------------------------------------------------------------------------

void cedar::dev::sensors::visual::Grabber::setName(const std::string& name)
{
  if (name != "")
  {
    _mName->setValue(name);
  }
  else
  {
    _mName->setValue(this->defaultGrabberName());
  }
}

//--------------------------------------------------------------------------------------------------------------------
const std::string& cedar::dev::sensors::visual::Grabber::getName() const
{
  return _mName->getValue();
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::onInit()
{
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::onCleanUp()
{
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::onGrab()
{
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::onDeclareParameters()
{
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::onWriteConfiguration()
{
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::onAddChannel()
{
  GrabberChannelPtr channel(new GrabberChannel);
  mChannels.push_back(channel);
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::readInit(unsigned int numCams)
{
  //specify grabber
  mNumCams = numCams;

  //default grabber name
  this->_mName->setValue(defaultGrabberName());

  //create number of channels
  for (unsigned int channel=0; channel < mNumCams; channel++)
  {
    this->onAddChannel();
  }

  mInstances.push_back(this);

  //Parameters in the derived classes
  if (!onDeclareParameters())
  {
    CEDAR_THROW
    (
      cedar::aux::InitializationException,
      "GrabberInterface::doInit - Error in onDeclareParameters in derived class"
    );
  }

  //restore configuration from config-file
  //@todo check loading of parameters!!!
  //cedar::aux::ConfigurationInterface::readOrDefaultConfiguration();
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::applyInit()
{
  //values from constructor (for guid) override settings from configfile

  //initialize the snapshot and recording names with default values
  //depends on no. of cameras
  setSnapshotName("snapshot.jpg");
  setRecordName("record.avi");

  //initialize derived class
  if (!onInit())
  {
    //cleanup already initialized channels
    doCleanUp();

    CEDAR_THROW
    (
      cedar::aux::InitializationException,
      "[GrabberInterface::doInit] ERROR: onInit in class" + this->getName()
    );
  }
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::writeConfiguration()
{    
  //update the parameters (if needed) in derived class
  if (!onWriteConfiguration())
  {
    cedar::aux::LogSingleton::getInstance()->error
                                             (
                                              this->getName() + "Error while updating the parameters "
                                                + "in the derived class. Configuration NOT saved!",
                                              "cedar::dev::sensors::visual::GrabberInterface::writeConfiguration()"
                                             );
    return false;
  }

  //write out the parameter of the Grabber
  //todo check save of configuration!!!
  //int result = cedar::aux::ConfigurationInterface::writeConfiguration();

  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             this->getName() + ": Configuration saved",
                                             "cedar::dev::sensors::visual::GrabberInterface::writeConfiguration()"
                                           );

  //return static_cast<bool>(result);
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
unsigned int cedar::dev::sensors::visual::Grabber::getNumCams() const
{
  return mNumCams;
}

//--------------------------------------------------------------------------------------------------------------------
double cedar::dev::sensors::visual::Grabber::getFpsMeasured() const
{
  return mFpsMeasured;
}

//--------------------------------------------------------------------------------------------------------------------
double cedar::dev::sensors::visual::Grabber::getFps() const
{
  double fps = 1000. / LoopedThread::getStepSize();
  return fps;
}


//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::setFps(double fps)
{
  bool wasRunning = QThread::isRunning();

  if (wasRunning)
  {
    stopGrabber();

    cedar::aux::LogSingleton::getInstance()->debugMessage
                                             (
                                               this->getName() + ": Grabbing stopped",
                                               "cedar::dev::sensors::visual::GrabberInterface::setFps()"
                                             );
  }

  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             this->getName()
                                               + ": Switch to " + boost::lexical_cast<std::string>(fps) + " fps",
                                             "cedar::dev::sensors::visual::GrabberInterface::setFps()"
                                           );

  //cycle time in ms: 1000ms/frames_per_second
  double milliseconds = 1000. / fps;

  //LoopedThread::_mStepSize = milliseconds;  //set param for configuration file (bug in LoopedThread)
  LoopedThread::setStepSize(milliseconds);        //change speed in thread

  if (wasRunning)
  {
    startGrabber();
    cedar::aux::LogSingleton::getInstance()->debugMessage
                                             (
                                               this->getName() + ": Grabbing started",
                                               "cedar::dev::sensors::visual::GrabberInterface::setFps()"
                                             );
  }

  std::string info;
  if (QThread::isRunning())
  {
    info =  ": Thread running";
  }
  else
  {
    info =  ": Thread not running";
  }
  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             this->getName() + info,
                                             "cedar::dev::sensors::visual::GrabberInterface::setFps()"
                                           );
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::stopGrabber()
{
  LoopedThread::stop();
  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             this->getName() + ": Grabbing stopped",
                                             "cedar::dev::sensors::visual::GrabberInterface::stopGrabber()"
                                           );
  mFpsMeasured = 0;
  if (mRecord)
  {
    stopRecording();
  }
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::startGrabber()
{
  mFpsMeasured = 0;
  mFpsCounter = 0;
  mFpsMeasureStart = boost::posix_time::microsec_clock::local_time();
  LoopedThread::start();
  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             this->getName() + ": Grabbing started with "
                                               + boost::lexical_cast<std::string>(getFps()) + " FPS.",
                                             "cedar::dev::sensors::visual::GrabberInterface::startGrabber()"
                                           );
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::grab()
{
  bool result = true;
  std::string error_info;

  mpReadWriteLock->lockForWrite();        //Lock
  try
  {
    result = onGrab();                    //Grab
  }
  catch (std::exception& e)
  {
    error_info = e.what();
    result = false;
  }
  mpReadWriteLock->unlock();              //Unlock

  if (! result)
  {
    doCleanUp();
    CEDAR_THROW
    ( 
      cedar::dev::sensors::visual::GrabberGrabException,
      "[GrabberInterface::grab] Error in grabber \""+ this->getName() + "\" on grabbing: " + error_info
    );
  }

  //grabbing ok, then
  //calculate fps every 5 (default) grabbing frames
  if (! (++mFpsCounter %= UPDATE_FPS_MEASURE_FRAME_COUNT))
  {
    mFpsMeasureStop = boost::posix_time::microsec_clock::local_time();

    //calculate real diff:
    boost::posix_time::time_duration measure_duration = mFpsMeasureStop - mFpsMeasureStart;
    mFpsMeasured = double(UPDATE_FPS_MEASURE_FRAME_COUNT * 1000000. / measure_duration.total_microseconds());

    mFpsMeasureStart = boost::posix_time::microsec_clock::local_time();
  }

  //check if recording is on
  if (mRecord)
  {
    for (unsigned int channel = 0; channel < mNumCams; ++channel)
    {
      try
      {
        getChannel(channel)->mVideoWriter << getChannel(channel)->mImageMat;
      }
      catch (std::exception& e)
      {
        std::stringstream info;
        info << "[GrabberInterface::grab] Error recording channel" << channel << ": " << e.what();
        doCleanUp();
        CEDAR_THROW(cedar::dev::sensors::visual::GrabberRecordingException,info.str());
      }
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------
cv::Mat cedar::dev::sensors::visual::Grabber::getImage(unsigned int channel) const
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::getImage");
  }
  return getChannel(channel)->mImageMat;
}

//--------------------------------------------------------------------------------------------------------------------
QReadWriteLock* cedar::dev::sensors::visual::Grabber::getReadWriteLockPointer() const
{
  return mpReadWriteLock;
}

//--------------------------------------------------------------------------------------------------------------------
std::string& cedar::dev::sensors::visual::Grabber::getSourceInfo(unsigned int channel)
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::getSourceInfo");
  }
  onUpdateSourceInfo(channel);
  return getChannel(channel)->mChannelInfo;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::setSnapshotName(const std::string& snapshotName)
{
  if (snapshotName == "")
  {
    return;
  }

  //initialize snapshot-names
  std::string name = snapshotName;
  std::string ext  = "";

  //check if extension in filename
  std::size_t pos = name.rfind(".");

  if (pos != std::string::npos)
  {
    //yes: extract it
    int cnt = pos;
    ext  = name.substr(pos);
    name = name.substr(0,cnt);
  }
  else
  {
    //no: use default
    ext  = mGrabberDefaultSnapshotExtension;
    name = snapshotName;
  }

  if (mNumCams==1)
  {
    getChannel(0)->mSnapshotName = name + ext;
  }
  else
  {
    for (unsigned int channel=0; channel<mNumCams;channel++)
    {
      getChannel(channel)->mSnapshotName = name + this->getChannelSaveFilenameAddition(channel) + ext;
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::setSnapshotName
(
  unsigned int channel,
  const std::string& snapshotName
)
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::setSnapshotName");
  }

  if (snapshotName != "")
  {
    //check if there is an extension
    std::string name = snapshotName;
    std::size_t pos = name.rfind(".");

    if (pos == std::string::npos)
    {
      //no: use default extension
      name.append(mGrabberDefaultSnapshotExtension);
    }
    getChannel(channel)->mSnapshotName = name;
  }
}

//--------------------------------------------------------------------------------------------------------------------
std::string cedar::dev::sensors::visual::Grabber::getSnapshotName(unsigned int channel) const
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::getSnapshotName");
  }
  return getChannel(channel)->mSnapshotName;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::saveSnapshot(unsigned int channel) const
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::saveSnapshot");
  }

  //copy image to local buffer for slow imwrite-function and for error-checking
  cv::Mat imgBuffer;

  if (!getChannel(channel)->mImageMat.empty())
  {
    mpReadWriteLock->lockForRead();
    getChannel(channel)->mImageMat.copyTo(imgBuffer);
    mpReadWriteLock->unlock();

    try
    {
      cv::imwrite(getChannel(channel)->mSnapshotName, imgBuffer);
    }
    catch (std::exception &e)
    {
      std::stringstream info;
      info << "[GrabberInterface::saveSnapshot] Channel " << channel << ": " << e.what();
      CEDAR_THROW(cedar::dev::sensors::visual::GrabberSnapshotException,info.str());
    }
  }
  else
  {
    std::string info = "[GrabberInterface::saveSnapshot] Channel "
                       + boost::lexical_cast<std::string>(channel)
                       +": Image matrix is empty!";
    CEDAR_THROW(cedar::dev::sensors::visual::GrabberSnapshotException,info);
  }
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::saveSnapshotAllCams() const
{
  for (unsigned int cam = 0; cam < mNumCams; ++cam)
  {
    saveSnapshot(cam);
  }
}

//--------------------------------------------------------------------------------------------------------------------
cv::Size cedar::dev::sensors::visual::Grabber::getSize(unsigned int channel) const
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::getSize");
  }
  return getChannel(channel)->mImageMat.size();
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::setRecordName(const std::string& recordName)
{
  if (recordName == "")
  {
    return;
  }

  //initialize snapshot-names
  std::string name = recordName;
  std::string ext  = "";

  //check if extension in filename
  std::size_t pos = name.rfind(".");

  if (pos != std::string::npos)
  {
    //yes: extract it
    ext  = name.substr(pos);
    name = name.substr(0,pos);
  }
  else
  {
    //no: use default
    ext  = mGrabberDefaultRecordExtension;
    name = recordName;
  }

  //filename depends on no. of cams
  if (mNumCams == 1)
  {
    getChannel(0)->mRecordName = name + ext;
  }
  else
  {
    for (unsigned int channel=0; channel<mNumCams;channel++)
    {
      getChannel(channel)->mRecordName = name + this->getChannelSaveFilenameAddition(channel) + ext;
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::setRecordName(unsigned int channel, const std::string& recordName )
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::setRecordName");
  }

  if (recordName != "")
  {
    //check if there is an extension
    std::string name = recordName;
    std::size_t pos = name.rfind(".");

    if (pos == std::string::npos)
    {
      //no: use default extension
      name.append(mGrabberDefaultRecordExtension);
    }
    getChannel(channel)->mRecordName = name;
  }
}

//--------------------------------------------------------------------------------------------------------------------
const std::string& cedar::dev::sensors::visual::Grabber::getRecordName(unsigned int channel) const
{
  if (channel >= mNumCams)
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"GrabberInterface::getRecordName");
  }
  return getChannel(channel)->mRecordName;
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::startRecording(double fps, int fourcc, bool color, bool startThread)
{
  if (mRecord)
  {
    cedar::aux::LogSingleton::getInstance()->warning
                                             (
                                               this->getName() + ": Grabber is already recording",
                                               "cedar::dev::sensors::visual::GrabberInterface::startRecording()"
                                             );
    return true;
  }

  //set the record-flag
  mRecord = true;
  unsigned int recording_channels = 0;

  //write the video-file with the actual grabbing-speed
  //this is independent from the speed of the avi-file or the camera framerate

  for (unsigned int channel = 0; channel < mNumCams; ++channel)
  {
    //create writer
    cv::VideoWriter writer(getChannel(channel)->mRecordName,fourcc,fps,getChannel(channel)->mImageMat.size(),color);

    if (writer.isOpened())
    {
      //save it in channel-struct
      getChannel(channel)->mVideoWriter = writer;

      std::string info = ": Channel " + boost::lexical_cast<std::string>(channel)
                         + " recording to " + getChannel(channel)->mRecordName;

      cedar::aux::LogSingleton::getInstance()->message
                                               (
                                                 this->getName() + info,
                                                 "cedar::dev::sensors::visual::GrabberInterface::startRecording()"
                                               );
      recording_channels++;
    }
  }

  if (recording_channels != mNumCams)
  {
    CEDAR_THROW
      (
        cedar::dev::sensors::visual::GrabberRecordingException,
        "Start recording: only " + boost::lexical_cast<std::string>(recording_channels)
          + " of " + boost::lexical_cast<std::string>(mNumCams) + " recording!"
      )
  }

  //startGrabber the grabberthread if needed
  if (!QThread::isRunning() && startThread)
  {
    cedar::aux::LogSingleton::getInstance()->message
                                             (
                                               this->getName() + ": Grabbing started on recording",
                                               "cedar::dev::sensors::visual::GrabberInterface::startRecording()"
                                             );
    mGrabberThreadStartedOnRecording = true;
    startGrabber();
  }
  return true;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::stopRecording()
{
  if (mRecord)
  {
    mRecord = false;
    if (mGrabberThreadStartedOnRecording)
    {
      stopGrabber();
      mGrabberThreadStartedOnRecording = false;
      cedar::aux::LogSingleton::getInstance()->debugMessage
                                               (
                                                 this->getName() + ": Grabbing stopped",
                                                 "cedar::dev::sensors::visual::GrabberInterface::stopRecording()"
                                               );
    }

    //delete the videowriter
    for (unsigned int channel=0; channel<mNumCams;channel++)
    {
      getChannel(channel)->mVideoWriter = cv::VideoWriter();
    }
  }
}

//--------------------------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::Grabber::isRecording() const
{
  return mRecord;
}

//--------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::Grabber::step(double)
{
  //if something went wrong on grabbing,
  //an exception is being thrown in the grab function
  //right after a cleanup
  grab();
}

//--------------------------------------------------------------------------------------------------------------------
std::string cedar::dev::sensors::visual::Grabber::getChannelSaveFilenameAddition(int channel) const
{
  return "_ch["+boost::lexical_cast<std::string>(channel)+"]";
}

