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

    File:        PictureGrabber.cpp

    Maintainer:  Georg Hartinger
    Email:       georg.hartinger@ini.rub.de
    Date:        2011 01 08

    Description: Implementation of the @em cedar::dev::sensors::visual::PictureGrabber class..

    Credits:

======================================================================================================================*/

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/devices/sensors/visual/PictureGrabber.h"
#include "cedar/auxiliaries/exceptions.h"
#include "cedar/auxiliaries/casts.h"

// SYSTEM INCLUDES


//----------------------------------------------------------------------------------------------------------------------
// register the class
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool declared
    = cedar::dev::sensors::visual::Grabber::ChannelManagerSingleton::getInstance()
        ->registerType<cedar::dev::sensors::visual::PictureGrabber::PictureChannelPtr>();
}

//----------------------------------------------------------------------------------------------------------------------
//constructors and destructor
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------
//Constructor for single-file grabber
cedar::dev::sensors::visual::PictureGrabber::PictureGrabber
(
  const std::string& pictureFileName
)
:
cedar::dev::sensors::visual::Grabber
(
  "PictureGraber",
  cedar::dev::sensors::visual::PictureGrabber::PictureChannelPtr
  (
    new cedar::dev::sensors::visual::PictureGrabber::PictureChannel(pictureFileName)
  )
)
{
  init();
}

//----------------------------------------------------------------------------------------------------
//Constructor for stereo-file grabber
cedar::dev::sensors::visual::PictureGrabber::PictureGrabber
(
  const std::string& pictureFileName0,
  const std::string& pictureFileName1
)
:
cedar::dev::sensors::visual::Grabber
(
  "StereoPictureGraber",
  cedar::dev::sensors::visual::PictureGrabber::PictureChannelPtr
  (
    new cedar::dev::sensors::visual::PictureGrabber::PictureChannel(pictureFileName0)
  ),
  cedar::dev::sensors::visual::PictureGrabber::PictureChannelPtr
  (
    new cedar::dev::sensors::visual::PictureGrabber::PictureChannel(pictureFileName1)
  )
)
{
  init();
}

//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::init()
{
  cedar::aux::LogSingleton::getInstance()->allocating(this);

  //watch filename on every channel
  for (unsigned int channel=0; channel<_mChannels->size(); ++channel)
  {
    QObject::connect
             (
               getPictureChannel(channel)->_mSourceFileName.get(),
               SIGNAL(valueChanged()),
               this,
               SLOT(fileNameChanged())
             );
  }

  //watch if channel added to ObjectParameterList
  _mChannels->connectToObjectAddedSignal
              (
                boost::bind(&cedar::dev::sensors::visual::PictureGrabber::channelAdded,this,_1)
              );

}
//----------------------------------------------------------------------------------------------------
cedar::dev::sensors::visual::PictureGrabber::~PictureGrabber()
{
  doCleanUp();
  cedar::aux::LogSingleton::getInstance()->freeing(this);
}

//--------------------------------------------------------------------------------------------------------------------
// configurable interface
//--------------------------------------------------------------------------------------------------------------------

//void cedar::dev::sensors::visual::PictureGrabber::readConfiguration(const cedar::aux::ConfigurationNode& node)
//{
//  //do readConfiguration for all childs. This is implemented in the base class
//  cedar::aux::Configurable::readConfiguration(node);
//
//  //initialize grabber:
//
//}

//--------------------------------------------------------------------------------------------------------------------
// slots
//--------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::channelAdded(int index)
{
  QObject::connect
           (
             getPictureChannel(index)->_mSourceFileName.get(),
             SIGNAL(valueChanged()),
             this,
             SLOT(fileNameChanged())
           );
}

//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::fileNameChanged()
{
  //get sender
  cedar::aux::FileParameterPtr p_sender
    = cedar::aux::FileParameterPtr(cedar::aux::asserted_cast<cedar::aux::FileParameter * >(QObject::sender()));

  //search for the changed channel
  unsigned int num_channels = getNumCams();
  unsigned int channel = 0;

  while ((channel < num_channels) && (p_sender != getPictureChannel(channel)->_mSourceFileName))
  {
    channel++;
  }

  //if found, read new image for that channel
  if (p_sender == getPictureChannel(channel)->_mSourceFileName)
  {
    const std::string filename = getPictureChannel(channel)->_mSourceFileName->getPath();

    //lock image-matrix for writing
    mpReadWriteLock->lockForWrite();
    getPictureChannel(channel)->mImageMat = cv::imread(filename);
    mpReadWriteLock->unlock();

    if (getPictureChannel(channel)->mImageMat.empty())
    {
      std::string message = this->getName() + ": Grabbing failed on Channel "
                            + boost::lexical_cast<std::string>(channel) + " from \"" + filename + "\"" ;
      cedar::aux::LogSingleton::getInstance()->error
                                               (
                                                 message,
                                                 "cedar::dev::sensors::visual::PictureGrabber::fileNameChanged()"
                                               );
      this->closeGrabber();
    }
    else
    {
      cedar::aux::LogSingleton::getInstance()->debugMessage
                                               (
                                                 this->getName() + ": New file successfully read.",
                                                 "cedar::dev::sensors::visual::PictureGrabber::fileNameChanged()"
                                               );
      mCaptureDeviceCreated = true;
      setChannelInfo(channel);
      emit pictureChanged();
    }
  }

}

//----------------------------------------------------------------------------------------------------------------------
// grabber
//----------------------------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::onCloseGrabber()
{
  //nothing to reset here
}

//----------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::PictureGrabber::onCreateGrabber()
{
  const std::string name = this->getName();
  unsigned int num_cams = getNumCams();

  //show debug message
  std::stringstream init_message;
  init_message << "Initialize picture grabber with " << getNumCams() << " channel(s) ...";

  for(unsigned int channel = 0; channel < num_cams; ++channel)
  {
    init_message << std::endl << "\tChannel " << channel << ": capture from Picture: "
                 << getPictureChannel(channel)->_mSourceFileName->getPath();
  }
  cedar::aux::LogSingleton::getInstance()->debugMessage
                                           (
                                             name + ": " + init_message.str(),
                                             "cedar::dev::sensors::visual::PictureGrabber::onInit()"
                                           );

  //for every channel, read from image-file
  bool result = true;

  for (unsigned int channel = 0; channel < num_cams; ++channel)
  {
    cv::Mat frame = cv::imread(getPictureChannel(channel)->_mSourceFileName->getPath());

    if (!frame.empty())
    {
      getPictureChannel(channel)->mImageMat = frame;
      setChannelInfo(channel);
    }
    else
    {
      result = false;
      cedar::aux::LogSingleton::getInstance()->error
                                               (
                                                  name + ": Grabbing failed on Channel "
                                                  + boost::lexical_cast<std::string>(channel) + " from \""
                                                  + getPictureChannel(channel)->_mSourceFileName->getPath() + "\"",
                                                "cedar::dev::sensors::visual::PictureGrabber::onInit()"
                                               );
    }
  }

  if (result)
  {
    emit pictureChanged();
  }

  return result;
}

//----------------------------------------------------------------------------------------------------
bool cedar::dev::sensors::visual::PictureGrabber::onGrab()
{
  bool result = true;
  unsigned int num_cams = getNumCams();
  for(unsigned int channel = 0; channel < num_cams; ++channel)
  {
    result = !(getPictureChannel(channel)->mImageMat.empty()) && result;
  }
  return result;
}

//----------------------------------------------------------------------------------------------------------------------
// picturegrabber
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::setSourceFile(unsigned int channel, const std::string& fileName)
{
  if (channel >= getNumCams())
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"PictureGrabber::setSourceFile");
  }

  // signal onChanged() connected to fileNameChanged()
  getPictureChannel(channel)->_mSourceFileName->setValue(fileName);
}

//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::setSourceFile(const std::string& fileName)
{
  setSourceFile(0,fileName);
}

//----------------------------------------------------------------------------------------------------
const std::string cedar::dev::sensors::visual::PictureGrabber::getSourceFile(unsigned int channel)
{
  if (channel >= getNumCams())
  {
    CEDAR_THROW(cedar::aux::IndexOutOfRangeException,"PictureGrabber::setSourceFile");
  }

  return getPictureChannel(channel)->_mSourceFileName->getPath();
}


//----------------------------------------------------------------------------------------------------
void cedar::dev::sensors::visual::PictureGrabber::setChannelInfo(unsigned int channel)
{
  getPictureChannel(channel)->mChannelInfo = getPictureChannel(channel)->_mSourceFileName->getPath();
}
