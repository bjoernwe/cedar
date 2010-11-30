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
 
 ----- Author:      Hendrik Reimann
 ----- Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 11 28
 
 ----- Description: unit test for the @em cedar::dev::robot::KinematicChainModel class.
 
 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "cedar/devices/robot/KinematicChainModel.h"
#include "cedar/auxiliaries/math/tools.h"
#include "cedar/auxiliaries/LogFile.h"

// SYSTEM INCLUDES

using namespace cedar::dev::robot;
using namespace cedar::aux;
using namespace cv;

int main()
{
  LogFile log_file("KinematicChainModel.log");
  log_file.addTimeStamp();
  log_file << std::endl;
  // the number of errors encountered in this test
  int errors = 0;
  
  // create instance of test class
  ReferenceGeometry reference_geometry("test.conf");
  KinematicChainModel test_arm_model(&reference_geometry);
  
  //--------------------------------------------------------------------------------------------------------------------
  // transformation
  //--------------------------------------------------------------------------------------------------------------------
  log_file << "test: transformations" << std::endl;
  cv::Mat theta = Mat::zeros( 4, 1, CV_64FC1 );
  theta.at<double>(2, 0) = -M_PI*0.5;
  theta.at<double>(3, 0) = M_PI*0.5;
  test_arm_model.calculateTransformations(theta);
  cv::Mat joint_transformation_1 = test_arm_model.jointTransformation(1);
  cv::Mat joint_transformation_3 = test_arm_model.jointTransformation(3);
  if (
      // transformation to joint 1 frame
      !IsZero(joint_transformation_1.at<double>(0, 0) - 1)
      || !IsZero(joint_transformation_1.at<double>(0, 1) - 0)
      || !IsZero(joint_transformation_1.at<double>(0, 2) - 0)
      || !IsZero(joint_transformation_1.at<double>(1, 0) - 0)
      || !IsZero(joint_transformation_1.at<double>(1, 1) - 1)
      || !IsZero(joint_transformation_1.at<double>(1, 2) - 0)
      || !IsZero(joint_transformation_1.at<double>(2, 0) - 0)
      || !IsZero(joint_transformation_1.at<double>(2, 1) - 0)
      || !IsZero(joint_transformation_1.at<double>(2, 2) - 1)
      || !IsZero(joint_transformation_1.at<double>(0, 3) - 0)
      || !IsZero(joint_transformation_1.at<double>(1, 3) - 0)
      || !IsZero(joint_transformation_1.at<double>(2, 3) - 2)
      || !IsZero(joint_transformation_1.at<double>(3, 0) - 0)
      || !IsZero(joint_transformation_1.at<double>(3, 1) - 0)
      || !IsZero(joint_transformation_1.at<double>(3, 2) - 0)
      || !IsZero(joint_transformation_1.at<double>(3, 3) - 1)
      // transformation to joint 3 frame
      || !IsZero(joint_transformation_3.at<double>(0, 0) - 1)
      || !IsZero(joint_transformation_3.at<double>(0, 1) - 0)
      || !IsZero(joint_transformation_3.at<double>(0, 2) - 0)
      || !IsZero(joint_transformation_3.at<double>(1, 0) - 0)
      || !IsZero(joint_transformation_3.at<double>(1, 1) - 1)
      || !IsZero(joint_transformation_3.at<double>(1, 2) - 0)
      || !IsZero(joint_transformation_3.at<double>(2, 0) - 0)
      || !IsZero(joint_transformation_3.at<double>(2, 1) - 0)
      || !IsZero(joint_transformation_3.at<double>(2, 2) - 1)
      || !IsZero(joint_transformation_3.at<double>(0, 3) - 0)
      || !IsZero(joint_transformation_3.at<double>(1, 3) - 2)
      || !IsZero(joint_transformation_3.at<double>(2, 3) - 4)
      || !IsZero(joint_transformation_3.at<double>(3, 0) - 0)
      || !IsZero(joint_transformation_3.at<double>(3, 1) - 0)
      || !IsZero(joint_transformation_3.at<double>(3, 2) - 0)
      || !IsZero(joint_transformation_3.at<double>(3, 3) - 1)
      )
  {
    errors++;
    log_file << "ERROR with calculateTransformations() or transformation()" << std::endl;
  }
//  log_file << joint_transformation_1.at<double>(0, 0) << " " << joint_transformation_1.at<double>(0, 1) << " " << joint_transformation_1.at<double>(0, 2) << " " << joint_transformation_1.at<double>(0, 3) << std::endl;
//  log_file << joint_transformation_1.at<double>(1, 0) << " " << joint_transformation_1.at<double>(1, 1) << " " << joint_transformation_1.at<double>(1, 2) << " " << joint_transformation_1.at<double>(1, 3) << std::endl;
//  log_file << joint_transformation_1.at<double>(2, 0) << " " << joint_transformation_1.at<double>(2, 1) << " " << joint_transformation_1.at<double>(2, 2) << " " << joint_transformation_1.at<double>(2, 3) << std::endl;
//  log_file << joint_transformation_1.at<double>(3, 0) << " " << joint_transformation_1.at<double>(3, 1) << " " << joint_transformation_1.at<double>(3, 2) << " " << joint_transformation_1.at<double>(3, 3) << std::endl;
//  
//  log_file << joint_transformation_3.at<double>(0, 0) << " " << joint_transformation_3.at<double>(0, 1) << " " << joint_transformation_3.at<double>(0, 2) << " " << joint_transformation_3.at<double>(0, 3) << std::endl;
//  log_file << joint_transformation_3.at<double>(1, 0) << " " << joint_transformation_3.at<double>(1, 1) << " " << joint_transformation_3.at<double>(1, 2) << " " << joint_transformation_3.at<double>(1, 3) << std::endl;
//  log_file << joint_transformation_3.at<double>(2, 0) << " " << joint_transformation_3.at<double>(2, 1) << " " << joint_transformation_3.at<double>(2, 2) << " " << joint_transformation_3.at<double>(2, 3) << std::endl;
//  log_file << joint_transformation_3.at<double>(3, 0) << " " << joint_transformation_3.at<double>(3, 1) << " " << joint_transformation_3.at<double>(3, 2) << " " << joint_transformation_3.at<double>(3, 3) << std::endl;
  
  //--------------------------------------------------------------------------------------------------------------------
  // Jacobians
  //--------------------------------------------------------------------------------------------------------------------
  log_file << "test: Jacobians" << std::endl;
  cv::Mat origin = Mat::zeros( 4, 1, CV_64FC1 );
  origin.at<double>( 3, 0 ) = 1;
  cv::Mat jacobian_1 = Mat::zeros(3, 4, CV_64FC1);
  cv::Mat jacobian_3 = Mat::zeros(3, 4, CV_64FC1);
  test_arm_model.jacobian(origin, 1, jacobian_1, KinematicChainModel::LOCAL_COORDINATES);
  test_arm_model.jacobian(origin, 3, jacobian_3, KinematicChainModel::LOCAL_COORDINATES);
  if (
      // Jacobian of joint 1
      !IsZero(jacobian_1.at<double>(0, 0) - 0)
      || !IsZero(jacobian_1.at<double>(1, 0) - -2)
      || !IsZero(jacobian_1.at<double>(2, 0) - 0)
      || !IsZero(jacobian_1.at<double>(0, 1) - 0)
      || !IsZero(jacobian_1.at<double>(1, 1) - 0)
      || !IsZero(jacobian_1.at<double>(2, 1) - 0)
      || !IsZero(jacobian_1.at<double>(0, 2) - 0)
      || !IsZero(jacobian_1.at<double>(1, 2) - 0)
      || !IsZero(jacobian_1.at<double>(2, 2) - 0)
      || !IsZero(jacobian_1.at<double>(0, 3) - 0)
      || !IsZero(jacobian_1.at<double>(1, 3) - 0)
      || !IsZero(jacobian_1.at<double>(2, 3) - 0)
      || !IsZero(norm(jacobian_1 - test_arm_model.jacobian(origin, 1, KinematicChainModel::LOCAL_COORDINATES)))
      // Jacobian of joint 3
      || !IsZero(jacobian_3.at<double>(0, 0) - 0)
      || !IsZero(jacobian_3.at<double>(1, 0) - -4)
      || !IsZero(jacobian_3.at<double>(2, 0) - 2)
      || !IsZero(jacobian_3.at<double>(0, 1) - 0)
      || !IsZero(jacobian_3.at<double>(1, 1) - -2)
      || !IsZero(jacobian_3.at<double>(2, 1) - 2)
      || !IsZero(jacobian_3.at<double>(0, 2) - 0)
      || !IsZero(jacobian_3.at<double>(1, 2) - 0)
      || !IsZero(jacobian_3.at<double>(2, 2) - 2)
      || !IsZero(jacobian_3.at<double>(0, 3) - 0)
      || !IsZero(jacobian_3.at<double>(1, 3) - 0)
      || !IsZero(jacobian_3.at<double>(2, 3) - 0)
      || !IsZero(norm(jacobian_3 - test_arm_model.jacobian(origin, 3, KinematicChainModel::LOCAL_COORDINATES)))
     )
  {
    errors++;
    log_file << "ERROR with jacobian()" << std::endl;
  }
  //  log_file << jacobian_1.at<double>(0, 0) << " " << jacobian_1.at<double>(0, 1) << " " << jacobian_1.at<double>(0, 2) << " " << jacobian_1.at<double>(0, 3) << std::endl;
  //  log_file << jacobian_1.at<double>(1, 0) << " " << jacobian_1.at<double>(1, 1) << " " << jacobian_1.at<double>(1, 2) << " " << jacobian_1.at<double>(1, 3) << std::endl;
  //  log_file << jacobian_1.at<double>(2, 0) << " " << jacobian_1.at<double>(2, 1) << " " << jacobian_1.at<double>(2, 2) << " " << jacobian_1.at<double>(2, 3) << std::endl;
  //  log_file << std::endl;
  //
  //  log_file << jacobian_3.at<double>(0, 0) << " " << jacobian_3.at<double>(0, 1) << " " << jacobian_3.at<double>(0, 2) << " " << jacobian_3.at<double>(0, 3) << std::endl;
  //  log_file << jacobian_3.at<double>(1, 0) << " " << jacobian_3.at<double>(1, 1) << " " << jacobian_3.at<double>(1, 2) << " " << jacobian_3.at<double>(1, 3) << std::endl;
  //  log_file << jacobian_3.at<double>(2, 0) << " " << jacobian_3.at<double>(2, 1) << " " << jacobian_3.at<double>(2, 2) << " " << jacobian_3.at<double>(2, 3) << std::endl;
  //  log_file << std::endl;
  
  log_file << "test: spatialJacobian" << std::endl;
  cv::Mat spatial_jacobian = test_arm_model.spatialJacobian();
  if (
      !IsZero(spatial_jacobian.at<double>(0, 0) - 0)
      || !IsZero(spatial_jacobian.at<double>(1, 0) - 0)
      || !IsZero(spatial_jacobian.at<double>(2, 0) - 0)
      || !IsZero(spatial_jacobian.at<double>(3, 0) - 1)
      || !IsZero(spatial_jacobian.at<double>(4, 0) - 0)
      || !IsZero(spatial_jacobian.at<double>(5, 0) - 0)

      || !IsZero(spatial_jacobian.at<double>(0, 1) - 0)
      || !IsZero(spatial_jacobian.at<double>(1, 1) - 2)
      || !IsZero(spatial_jacobian.at<double>(2, 1) - 0)
      || !IsZero(spatial_jacobian.at<double>(3, 1) - 1)
      || !IsZero(spatial_jacobian.at<double>(4, 1) - 0)
      || !IsZero(spatial_jacobian.at<double>(5, 1) - 0)

      || !IsZero(spatial_jacobian.at<double>(0, 2) - 0)
      || !IsZero(spatial_jacobian.at<double>(1, 2) - 4)
      || !IsZero(spatial_jacobian.at<double>(2, 2) - 0)
      || !IsZero(spatial_jacobian.at<double>(3, 2) - 1)
      || !IsZero(spatial_jacobian.at<double>(4, 2) - 0)
      || !IsZero(spatial_jacobian.at<double>(5, 2) - 0)

      || !IsZero(spatial_jacobian.at<double>(0, 3) - 0)
      || !IsZero(spatial_jacobian.at<double>(1, 3) - 4)
      || !IsZero(spatial_jacobian.at<double>(2, 3) - -2)
      || !IsZero(spatial_jacobian.at<double>(3, 3) - 1)
      || !IsZero(spatial_jacobian.at<double>(4, 3) - 0)
      || !IsZero(spatial_jacobian.at<double>(5, 3) - 0)
     )
  {
    errors++;
    log_file << "ERROR with spatialJacobian()" << std::endl;
  }
  
//  log_file << spatial_jacobian.at<double>(0, 0) << " " << spatial_jacobian.at<double>(0, 1) << " " << spatial_jacobian.at<double>(0, 2) << " " << spatial_jacobian.at<double>(0, 3) << std::endl;
//  log_file << spatial_jacobian.at<double>(1, 0) << " " << spatial_jacobian.at<double>(1, 1) << " " << spatial_jacobian.at<double>(1, 2) << " " << spatial_jacobian.at<double>(1, 3) << std::endl;
//  log_file << spatial_jacobian.at<double>(2, 0) << " " << spatial_jacobian.at<double>(2, 1) << " " << spatial_jacobian.at<double>(2, 2) << " " << spatial_jacobian.at<double>(2, 3) << std::endl;
//  log_file << spatial_jacobian.at<double>(3, 0) << " " << spatial_jacobian.at<double>(3, 1) << " " << spatial_jacobian.at<double>(3, 2) << " " << spatial_jacobian.at<double>(3, 3) << std::endl;
//  log_file << spatial_jacobian.at<double>(4, 0) << " " << spatial_jacobian.at<double>(4, 1) << " " << spatial_jacobian.at<double>(4, 2) << " " << spatial_jacobian.at<double>(4, 3) << std::endl;
//  log_file << spatial_jacobian.at<double>(5, 0) << " " << spatial_jacobian.at<double>(5, 1) << " " << spatial_jacobian.at<double>(5, 2) << " " << spatial_jacobian.at<double>(5, 3) << std::endl;
//  log_file << std::endl;
  
  //--------------------------------------------------------------------------------------------------------------------
  // end-effector position
  //--------------------------------------------------------------------------------------------------------------------
  log_file << "test: endEffectorPosition" << std::endl;
  cv::Mat end_effector_position = test_arm_model.endEffectorPosition();
  if (
      !IsZero(end_effector_position.at<double>(0, 0) - 0)
      || !IsZero(end_effector_position.at<double>(1, 0) - 2)
      || !IsZero(end_effector_position.at<double>(2, 0) - 6)
      || !IsZero(end_effector_position.at<double>(3, 0) - 1)
     )
  {
    errors++;
    log_file << "ERROR with endEffectorPosition()" << std::endl;
  }
  
  //--------------------------------------------------------------------------------------------------------------------
  // end-effector transformation
  //--------------------------------------------------------------------------------------------------------------------
  log_file << "test: endEffectorTransformation" << std::endl;
  cv::Mat end_effector_transformation = test_arm_model.endEffectorTransformation();
  if (
      !IsZero(end_effector_transformation.at<double>(0, 0) - 1)
      || !IsZero(end_effector_transformation.at<double>(0, 1) - 0)
      || !IsZero(end_effector_transformation.at<double>(0, 2) - 0)
      || !IsZero(end_effector_transformation.at<double>(1, 0) - 0)
      || !IsZero(end_effector_transformation.at<double>(1, 1) - 1)
      || !IsZero(end_effector_transformation.at<double>(1, 2) - 0)
      || !IsZero(end_effector_transformation.at<double>(2, 0) - 0)
      || !IsZero(end_effector_transformation.at<double>(2, 1) - 0)
      || !IsZero(end_effector_transformation.at<double>(2, 2) - 1)
      || !IsZero(end_effector_transformation.at<double>(0, 3) - 0)
      || !IsZero(end_effector_transformation.at<double>(1, 3) - 2)
      || !IsZero(end_effector_transformation.at<double>(2, 3) - 6)
      || !IsZero(end_effector_transformation.at<double>(3, 0) - 0)
      || !IsZero(end_effector_transformation.at<double>(3, 1) - 0)
      || !IsZero(end_effector_transformation.at<double>(3, 2) - 0)
      || !IsZero(end_effector_transformation.at<double>(3, 3) - 1)
     )
  {
    errors++;
    log_file << "ERROR with endEffectorTransformation()" << std::endl;
  }
  
//  log_file << end_effector_transformation.at<double>(0, 0) << " " << end_effector_transformation.at<double>(0, 1) << " " << end_effector_transformation.at<double>(0, 2) << " " << end_effector_transformation.at<double>(0, 3) << std::endl;
//  log_file << end_effector_transformation.at<double>(1, 0) << " " << end_effector_transformation.at<double>(1, 1) << " " << end_effector_transformation.at<double>(1, 2) << " " << end_effector_transformation.at<double>(1, 3) << std::endl;
//  log_file << end_effector_transformation.at<double>(2, 0) << " " << end_effector_transformation.at<double>(2, 1) << " " << end_effector_transformation.at<double>(2, 2) << " " << end_effector_transformation.at<double>(2, 3) << std::endl;
//  log_file << end_effector_transformation.at<double>(3, 0) << " " << end_effector_transformation.at<double>(3, 1) << " " << end_effector_transformation.at<double>(3, 2) << " " << end_effector_transformation.at<double>(3, 3) << std::endl;
//  log_file << std::endl;
  
  //--------------------------------------------------------------------------------------------------------------------
  // end-effector jacobian
  //--------------------------------------------------------------------------------------------------------------------
  log_file << "test: endEffectorJacobian" << std::endl;
  cv::Mat end_effector_jacobian = test_arm_model.endEffectorJacobian();
  if (
      !IsZero(end_effector_jacobian.at<double>(0, 0) - 0)
      || !IsZero(end_effector_jacobian.at<double>(1, 0) - -6)
      || !IsZero(end_effector_jacobian.at<double>(2, 0) - 2)
      || !IsZero(end_effector_jacobian.at<double>(0, 1) - 0)
      || !IsZero(end_effector_jacobian.at<double>(1, 1) - -4)
      || !IsZero(end_effector_jacobian.at<double>(2, 1) - 2)
      || !IsZero(end_effector_jacobian.at<double>(0, 2) - 0)
      || !IsZero(end_effector_jacobian.at<double>(1, 2) - -2)
      || !IsZero(end_effector_jacobian.at<double>(2, 2) - 2)
      || !IsZero(end_effector_jacobian.at<double>(0, 3) - 0)
      || !IsZero(end_effector_jacobian.at<double>(1, 3) - -2)
      || !IsZero(end_effector_jacobian.at<double>(2, 3) - 0)
     )
  {
    errors++;
    log_file << "ERROR with endEffectorJacobian()" << std::endl;
  }
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  //--------------------------------------------------------------------------------------------------------------------
  // wire frame
  //--------------------------------------------------------------------------------------------------------------------
//  log_file << "test: wire frame" << std::endl;
//  object.drawAsWireFrame(true);
//  if (!object.isDrawnAsWireFrame())
//  {
//    errors++;
//    log_file << "ERROR with wire frame" << std::endl;
//  }
  
  
  
  

  
//  cv::Mat origin = Mat::zeros( 4, 1, CV_64FC1 );
//  origin.at<double>( 2, 0 ) = 2;
//  origin.at<double>( 3, 0 ) = 1;
//  cv::Mat J = Mat::zeros( 3, 4, CV_64FC1 );
//  test_arm_model.jacobian( origin, 3, J, KinematicChainModel::LOCAL_COORDINATES );
  
  //  cout << "end-effector transformation" << endl;
  //  write( test_arm.endEffectorTransformation() );
  //  cout << "end-effector position" << endl;
  //  write( test_arm.endEffectorPosition() );
  
  //  cout << "end-effector jacobian in world frame" << endl;
  //  write( J );
  
//  Mat p = test_arm.endEffectorPosition();
//  test_arm.jacobian( p, 3, J, KinematicChainModel::WORLD_COORDINATES );
//  cout << "end-effector jacobian in world frame" << endl;
//  write( J );
//  
//  cout << "end-effector jacobian in world frame, by convenience function" << endl;
//  write( test_arm.endEffectorJacobian() );
  
  //  cout << "spatial jacobian" << endl;
  //  write( test_arm.spatialJacobian() );
  
  
  log_file << "test finished, there were " << errors << " errors" << std::endl;
  if (errors > 255)
  {
    errors = 255;
  }
  return errors;
}











