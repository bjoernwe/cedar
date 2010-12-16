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

 ----- File:        main.cpp

 ----- Maintainer:  Hendrik Reimann
 ----- Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 11 17

 ----- Description: interactive test for the kinematic chain simulation

 ----- Credits:
 -----------------------------------------------------------------------------*/

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "devices/robot/SimulatedKinematicChain.h"
#include "devices/robot/KinematicChainModel.h"
#include "devices/robot/gl/CoraArm.h"
#include "auxiliaries/gl/Scene.h"
#include "auxiliaries/gui/Viewer.h"
#include "auxiliaries/gui/SceneWidget.h"


// SYSTEM INCLUDES
#include <QApplication>

using namespace std;
using namespace cv;
using namespace cedar::aux::gl;
using namespace cedar::aux::gui;
using namespace cedar::dev::robot::gl;
using namespace cedar::dev::robot;


int main(int argc, char **argv)
{
  QApplication a(argc, argv);

  // create simulated arm
  cedar::dev::robot::KinematicChainPtr p_test_arm(new SimulatedKinematicChain(std::string("../../../tests/interactive/devices/gl/Cora/cora_arm.conf")));

  // create model of simulated arm
  KinematicChainModelPtr p_test_arm_model(new KinematicChainModel(p_test_arm));

  // create gl visualization object
  cedar::dev::robot::gl::KinematicChainPtr p_test_arm_visualization(new cedar::dev::robot::gl::CoraArm(p_test_arm_model));

  // create scene and viewer to display the arm
  ScenePtr p_scene(new cedar::aux::gl::Scene);
  p_scene->setSceneLimit(2);
  p_scene->drawFloor(true);

  cedar::aux::gl::ObjectPtr p_object = p_test_arm_visualization;
  p_scene->addObject(p_object);

  // create a simple viewer for the scene
  Viewer viewer(p_scene);
  viewer.show();
  viewer.setSceneRadius(p_scene->getSceneLimit());

  // create a widget to control the scene
  SceneWidgetPtr p_scene_widget(new SceneWidget(p_scene));
//  p_scene_widget->show();

  p_test_arm->setJointAcceleration(0, .03);
  p_test_arm->setJointAcceleration(1, -.045);
  p_test_arm->setJointAcceleration(2, -.015);
  p_test_arm->setJointAcceleration(3, .025);
  p_test_arm->setJointAcceleration(4, .01);
  p_test_arm->setJointAcceleration(5, -.015);
  p_test_arm->setJointAcceleration(6, -.011);
  p_test_arm->setJointAcceleration(7, .005);

  p_test_arm->start();
  p_test_arm_model->startTimer(50.0);
  viewer.startTimer(50);
  a.exec();

  p_test_arm->stop();
  sleep(1);

  return 0;
}
