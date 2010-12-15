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

    File:        CoraKinematicChain.h

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 15

    Description: header for a class visualizing the arm of CoRA

    Credits:

======================================================================================================================*/

#ifndef CEDAR_DEV_ROBOT_GL_CORA_KINEMATIC_CHAIN_H
#define CEDAR_DEV_ROBOT_GL_CORA_KINEMATIC_CHAIN_H

// LOCAL INCLUDES
#include "devices/robot/gl/namespace.h"
#include "devices/robot/gl/KinematicChain.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES


/*!@brief Visualization of one of the CoRA arm
 *
 * This class provides a simple OpenGL visualization of the CoRA arm. It has to be provided with a pointer to an
 * instance of KinematicChainModel of the CoRA arm, used to get the transformations to the joint coordinate frames.
 * To actually display the arm, add an instance of this class to a scene (cedar::aux::gl::Scene) and create a viewer
 * for that scene (cedar::aux::gl::Viewer).
 */
class cedar::dev::robot::gl::CoraKinematicChain : public cedar::dev::robot::gl::KinematicChain
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief constructor
  CoraKinematicChain(cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel);
  //!@brief destructor
  ~CoraKinematicChain();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief draws the base
  virtual void drawBase() = 0;

  //!@brief draws the segment attached to the specified joint
  virtual void drawSegment(unsigned int index) = 0;

  //!@brief draws the end-effector
  virtual void drawEndEffector() = 0;

  //!@brief sets the material for gl visualization
  void setMaterial(int material);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  enum MaterialType {
                      NO_MATERIAL,
                      DARK_BLUE_METAL,
                      LIGHT_BLUE_METAL,
                      BRASS,
                      ARTIFICIAL_SKIN,
                      WHITE_PLASTIC,
                      BLACK_METAL,
                      CHROME
                    };
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  // none yet

}; // class cedar::dev::robot::gl::CoraKinematicChain

#endif // CEDAR_DEV_ROBOT_GL_CORA_KINEMATIC_CHAIN_H

