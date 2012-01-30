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

 ----- Description: interactive test for the gl functionalities

 ----- Credits:
 -----------------------------------------------------------------------------*/

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "cedar/auxiliaries/gl/Scene.h"
#include "cedar/auxiliaries/gui/SceneWidget.h"
#include "cedar/auxiliaries/gui/Viewer.h"
#include "cedar/auxiliaries/gl/Block.h"
#include "cedar/auxiliaries/gl/Sphere.h"
#include "cedar/auxiliaries/gl/Cone.h"
#include "cedar/auxiliaries/gl/Cylinder.h"
#include "cedar/auxiliaries/gl/Pyramid.h"
#include "cedar/auxiliaries/gl/Prism.h"
#include "cedar/auxiliaries/gl/Torus.h"
#include "cedar/auxiliaries/gl/Ellipse.h"
#include "cedar/auxiliaries/gl/Chessboard.h"

// SYSTEM INCLUDES
#include <QApplication>


int main(int argc, char **argv)
{
  QApplication a(argc, argv);

  // create the scene administering the objects to be visualized
  cedar::aux::gl::ScenePtr p_scene(new cedar::aux::gl::Scene);
  p_scene->setSceneLimit(10);
  p_scene->drawFloor(false);

  // create a simple viewer for the scene
  cedar::aux::gui::Viewer viewer(p_scene);
  viewer.show();
  viewer.setSceneRadius(10);
  viewer.startTimer(50);

  // create a rectangular block and add it to the scene
  cedar::aux::ObjectPtr p_block_object(new cedar::aux::Object());
  p_block_object->setPosition(3, -3, 3);
  p_block_object->setName(std::string("Bernd the Block"));
  cedar::aux::gl::ObjectPtr p_block(new cedar::aux::gl::Block(p_block_object, 1, 2, 3, 0, 1, 0.5));
  p_scene->addObject(p_block);
  p_block_object->rotate(0, M_PI/2);

  // create a sphere visualization and add it to the scene
  cedar::aux::ObjectPtr p_sphere_object(new cedar::aux::Object());
  p_sphere_object->setPosition(3, 3, 3);
  p_sphere_object->setName(std::string("Susi the Sphere"));
  cedar::aux::gl::ObjectPtr p_sphere(new cedar::aux::gl::Sphere(p_sphere_object, 1, 1, 0, 0.5));
  p_scene->addObject(p_sphere);

  // create a cone visualization and add it to the scene
  cedar::aux::ObjectPtr p_cone_object(new cedar::aux::Object());
  p_cone_object->setPosition(-3, 3, 3);
  p_cone_object->setName(std::string("Carl the Cone"));
  cedar::aux::gl::ObjectPtr p_cone(new cedar::aux::gl::Cone(p_cone_object, 1, 3, 0, 0, 1));
  p_scene->addObject(p_cone);

  // create a cylinder visualization and add it to the scene
  cedar::aux::ObjectPtr p_cylinder_object(new cedar::aux::Object());
  p_cylinder_object->setPosition(-3, -3, 3);
  p_cylinder_object->setName(std::string("Cindy the Cylinder"));
  cedar::aux::gl::ObjectPtr p_cylinder(new cedar::aux::gl::Cylinder(p_cylinder_object, 1, 3, 0, 1, 1));
  p_scene->addObject(p_cylinder);

  // create a pyramid visualization and add it to the scene
  cedar::aux::ObjectPtr p_pyramid_object(new cedar::aux::Object());
  p_pyramid_object->setPosition( 9, -3, 3 );
  p_pyramid_object->setName(std::string("Peter the Pyramid"));
  cedar::aux::gl::ObjectPtr p_pyramid(new cedar::aux::gl::Pyramid(p_pyramid_object, 2, 1.5, 1.5, 1, 1, 0));
  p_scene->addObject(p_pyramid);

  // create a torus visualization and add it to the scene
  cedar::aux::ObjectPtr p_prism_object(new cedar::aux::Object());
  p_prism_object->setPosition(0, 0, 5);
  p_prism_object->setName(std::string("Pamela Prisma"));
  cedar::aux::gl::ObjectPtr p_prism(new cedar::aux::gl::Prism(p_prism_object, 2, 1));
  p_scene->addObject(p_prism);

  // create a torus visualization and add it to the scene
  cedar::aux::ObjectPtr p_torus_object(new cedar::aux::Object());
  p_torus_object->setPosition( -7.5, 3, 3 );
  p_torus_object->setName(std::string("Tom the Torus"));
  cedar::aux::gl::ObjectPtr p_torus(new cedar::aux::gl::Torus(p_torus_object, 2, 0.3, 1, 0.5, 0));
  p_scene->addObject(p_torus);

  // create a torus visualization and add it to the scene
  cedar::aux::ObjectPtr p_ellipse_object(new cedar::aux::Object());
  p_ellipse_object->setPosition(-7.5, -3, 3);
  p_ellipse_object->setName(std::string("Elfriede la Ellipse"));
  cedar::aux::gl::ObjectPtr p_ellipse(new cedar::aux::gl::Ellipse(p_ellipse_object, 1, 2, 0.3, 1, 1, 0));
  p_scene->addObject(p_ellipse);

  // create a chessboard visualization and add it to the scene
  cedar::aux::ObjectPtr p_chessboard_object(new cedar::aux::Object());
  p_chessboard_object->setPosition( 7.5, 1.5, 3 );
  p_chessboard_object->setName(std::string("Caspar the checkered Chessboard"));
  cedar::aux::gl::ObjectPtr p_chessboard(new cedar::aux::gl::Chessboard(p_chessboard_object, 3, 3, 0.2, 8, 8, 1, 1, 1, 0, 0, 0));
  p_scene->addObject(p_chessboard);

  // create a widget to control the scene
  cedar::aux::gui::SceneWidgetPtr p_scene_widget(new cedar::aux::gui::SceneWidget(p_scene));
  p_scene_widget->show();

  a.exec();
  return 0;
}
