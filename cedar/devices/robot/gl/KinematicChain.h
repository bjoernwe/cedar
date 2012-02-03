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

 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik
 
 ----- File:        KinematicChain.h
 
 ----- Maintainer:  Hendrik Reimann
 ----- Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 11 06
 
 ----- Description: Header for the \em cedar::dev::robot::gl::KinematicChain class.
 
 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_DEV_ROBOT_GL_KINEMATIC_CHAIN_H
#define CEDAR_DEV_ROBOT_GL_KINEMATIC_CHAIN_H

// CEDAR INCLUDES
#include "cedar/devices/robot/gl/namespace.h"
#include "cedar/auxiliaries/gl/Object.h"
#include "cedar/devices/robot/KinematicChainModel.h"

// SYSTEM INCLUDES
#include <opencv2/opencv.hpp>

/*!@brief Visualization of a kinematic chain
 *
 * This class provides a simple OpenGL visualization of any kinematic chain. It has to be provided with a pointer to an 
 * instance of KinematicChainModel, used to get the transformations to the joint coordinate frames. To actually display
 * the chain, add an instance of this class to a scene (cedar::aux::gl::Scene) and create a viewer for that scene 
 * (cedar::aux::gl::Viewer). 
 */
class cedar::dev::robot::gl::KinematicChain : public cedar::aux::gl::Object
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief constructor
  KinematicChain(cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel);
  //!@brief destructor
  ~KinematicChain();
  
  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief draws a visualization of the object in the current GL context
  void draw(void);
  
  //!@brief flags the drawing state of the end-effector velocity vector
  void setDisplayEndEffectorVelocity(bool state);

  //!@brief flags the drawing state of the end-effector acceleration vector
  void setDisplayEndEffectorAcceleration(bool state);


  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief draws the base
  virtual void drawBase();

  //!@brief draws the segment attached to the specified joint
  virtual void drawSegment(unsigned int index);

  //!@brief draws the end-effector
  virtual void drawEndEffector();

  //!@brief draws the end-effector velocity vector
  void drawEndEffectorVelocity();

  //!@brief draws the end-effector acceleration vector
  void drawEndEffectorAcceleration();
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //! model of the kinematics, used for calculating transformations to the joint and end-effector frames
  cedar::dev::robot::KinematicChainModelPtr mpKinematicChainModel;

  //! decides whether the end-effector velocity will be drawn
  bool mIsDrawingEndEffectorVelocity;
  //! decides whether the end-effector acceleration will be drawn
  bool mIsDrawingEndEffectorAcceleration;

}; // class cedar::dev::robot::gl::KinematicChain
#endif // CEDAR_DEV_ROBOT_GL_KINEMATIC_CHAIN_H
