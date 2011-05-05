/*--------------------------------------------------------------------------
 ----- Institute:   	Ruhr-Universitaet-Bochum
 Institut fuer Neuroinformatik

 ----- File:			iiClosedFormInverseKinematics.cpp

 ----- Author:			Ioannis Iossifidis
 iossifidis@neuroinformatik.rub.de

 ----- Date:       	13 Σεπ 2009

 ----- Description:

 ----- Copyright:   	(c) Ioannis Iossifidis 13 Σεπ 2009

 ----- Warranties:		Of course, there's NO WARRANTY OF ANY KIND :-)

 ----- Credits:

 ----- Project:		hrOpenChainManipulatorLib

 --------------------------------------------------------------------------*/

#include <iostream>
#include "iiClosedFormInverseKinematics.hpp"

using namespace std;
using namespace cv;

ClosedFormInverseKinematics::ClosedFormInverseKinematics()
{
  InitVariables();
}


ClosedFormInverseKinematics::~ClosedFormInverseKinematics()
{
	// TODO Auto-generated destructor stub
}


void ClosedFormInverseKinematics::InitVariables()
{
  mP_u = Mat::zeros(3, 1, CV_64FC1);	// uperarm vector
  mP_f = Mat::zeros(3, 1, CV_64FC1);	// forearm vector
  mP_h = Mat::zeros(3, 1, CV_64FC1);	// hand vector
  mP_g = Mat::zeros(3, 1, CV_64FC1);	// gripper vector
  mP_W = Mat::zeros(3, 1, CV_64FC1);	// wrist vector
  mP_T = Mat::zeros(3, 1, CV_64FC1);	// vector to the endeffector

  /* mJointAngle(0) denotes the trunk angle
   * and mJointAngle(1) to mJointAngle(8) the angles for the
   * remaining joints*/
  mJointAngle = Mat::zeros(8, 1, CV_64FC1);

  /* This should replaced by an configFile in odrder to handle flexible different robotarm configurations*/
  mTrunk    = Mat::zeros(3, 1, CV_64FC1);
  mShoulder	= Mat::zeros(3, 1, CV_64FC1);
  mUperArm	= Mat::zeros(3, 1, CV_64FC1);
  mForeArm	= Mat::zeros(3, 1, CV_64FC1);
  mEef		= Mat::zeros(3, 1, CV_64FC1);

  mTrunk.at<double>(2, 0)    = 400;//420;
  mShoulder.at<double>(0, 0) = 225;
  mUperArm.at<double>(0, 0)	 = 325;
  mForeArm.at<double>(0, 0)	 = 310;
  mEef.at<double>(0, 0)	     = 285;

  //mTrunk.StdOutFormatted();
  //mShoulder.StdOutFormatted();
  //mUperArm.StdOutFormatted();
  //mForeArm.StdOutFormatted();
  //mEef.StdOutFormatted();

  /*Setting taskcoordiantes for testing*/
  /* Task coordinates
   * Pos in 		[mm]
   * Angles in 	[deg]*/
  mTaskCoordinates.Pos = Mat::zeros(3, 1, CV_64FC1);
  mTaskCoordinates.eefOrientationAngle = Mat::zeros(3, 1, CV_64FC1);

  //mTaskCoordinates.Pos(0) 				= 225;
  //mTaskCoordinates.Pos(1) 				= 500;
  //mTaskCoordinates.Pos(2) 				= 100;
  //mTaskCoordinates.eefOrientationAngle	=	0;
  //mTaskCoordinates.redundancyAng			=   180;
  //mTaskCoordinates.shoulderGear 			= 	30;
  //mTaskCoordinates.trunkAng				= 	0;


  //TaskCoordinatesToArmGeometry();
  //InverseKinematics();

  return;
}


void ClosedFormInverseKinematics::Rx(double rotAngle, Mat* pRotMatX)
{

  (* pRotMatX ) = Mat::zeros(3, 3, CV_64FC1);

  (* pRotMatX ).at<double>(0,0) = 1;
  (* pRotMatX ).at<double>(1,1) = cos(rotAngle);
  (* pRotMatX ).at<double>(1,2) = -sin(rotAngle);
  (* pRotMatX ).at<double>(2,1) = sin(rotAngle);
  (* pRotMatX ).at<double>(2,2) = cos(rotAngle);
  //printf("RotMatX\n");
  //(* pRotMatX ).StdOutFormatted();

  return;
}


void ClosedFormInverseKinematics::Ry(double rotAngle, Mat* pRotMatY)
{

  (* pRotMatY ) = Mat::zeros(3, 3, CV_64FC1);

  (* pRotMatY ).at<double>(0,0) = cos(rotAngle);
  (* pRotMatY ).at<double>(0,2) = sin(rotAngle);
  (* pRotMatY ).at<double>(1,1) = 1;
  (* pRotMatY ).at<double>(2,0) = -sin(rotAngle);
  (* pRotMatY ).at<double>(2,2) = cos(rotAngle);

  //printf("RotMatY\n");
  //(* pRotMatY ).StdOutFormatted();

  return;
}


void ClosedFormInverseKinematics::Rz(double rotAngle, Mat* pRotMatZ)
{

  (*pRotMatZ) = Mat::zeros(3, 3, CV_64FC1);


  (*pRotMatZ).at<double>(0,0) = cos(rotAngle);
  (*pRotMatZ).at<double>(0,1) = -sin(rotAngle);
  (*pRotMatZ).at<double>(1,0) = sin(rotAngle);
  (*pRotMatZ).at<double>(1,1) = cos(rotAngle);
  (*pRotMatZ).at<double>(2,2) = 1;
  //printf("RotMatX\n");
  //(*pRotMatZ).StdOutFormatted();

  return;
}


int ClosedFormInverseKinematics::TaskCoordinatesToArmGeometry()
{
	Mat eefVector = Mat::zeros(3,1,CV_64FC1);
	Mat rotX = Mat::zeros(3,3,CV_64FC1);
	Mat rotX2 = Mat::zeros(3,3,CV_64FC1);
	Mat rotY = Mat::zeros(3,3,CV_64FC1);
  Mat rotY2 = Mat::zeros(3,3,CV_64FC1);
	Mat rotZ = Mat::zeros(3,3,CV_64FC1);
	Mat e_z_unit = Mat::zeros(3,1,CV_64FC1);

  e_z_unit.at<double>(2, 0) = 1;

  double trunk_ang = CalcTrunkAng();

  /* assign mJointAngle(0) with the calculated trunk angle*/
  mJointAngle.at<double>(0, 0) = trunk_ang;

  double shoulder_gear = deg2rad(mTaskCoordinates.shoulderGear);

  /*calculate the endeffector position with respect to the trunk orientation
   * and the shouldergear.*/
  Rz(-trunk_ang,&rotZ);

  mCalculatedHandState.eefPos = rotZ * mTaskCoordinates.Pos;

  /* different offset
   * 1. offset generated by the shoulderGear
   * 2. offset generated by the trunk length*/
  Ry(-shoulder_gear,&rotY);
  // the following line prevents the next from crashing. i don't know why!
  mCalculatedHandState.eefPos = mCalculatedHandState.eefPos - (rotY * mShoulder) - mTrunk;

  Ry(shoulder_gear,&rotY);
  mCalculatedHandState.eefPos = rotY * mCalculatedHandState.eefPos;


  /* Endeffector  orientation
   * we do this due to our own convention of angles determine the
   * orientation of robots hand. It is not necessary for the overall solution*/
  double phi_eef	 = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(0, 0)) - M_PI_2;
  double theta_eef = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(1, 0)) + M_PI;

  /* From this section the calculation are related to the attached technical report*/

  // eef vector
  Mat p_m = Mat::zeros(3,1,CV_64FC1);// vector to middle of the redundancy circle
  //CvMAT p_w = CvMAT(3,1,CV_64FC1);// wrist vector


  mP_T = mCalculatedHandState.eefPos; // using the offset cleaned endeffctor postion



  /* calculate wrist position */
  Rz(phi_eef - trunk_ang,&rotZ);
  Ry(theta_eef,&rotY);
  Ry(shoulder_gear,&rotY2);

  /* wirst postion is calculated by subtracting the vector, defining the orinetation and length
   * of the hand */
  //mCalculatedHandState.wristPos = mCalculatedHandState.eefPos - rotY2 * rotZ * rotY * mEef;
  //mP_W =  mP_T - rotY2 * rotZ * rotY * mEef;
  mP_W =  mP_T -  rotZ * rotY * mEef;

  //printf(" mP_W.StdOutFormatted(); \n");
  //mP_W.StdOutFormatted();

  /* calculation the elbow position with respect to the redundancy angle*/
  double pw_norm_pow2 = pow(norm(mP_W, CV_L2), 2);//between shoulder and wrist
  double pu_norm_pow2	= pow(norm(mUperArm, CV_L2), 2);
  double pf_norm_pow2	= pow(norm(mForeArm, CV_L2), 2);

  //cout<<"mP_W Norm = " << mP_W.norm(CV_L2)<<endl;

  double auxiliary_variable =  (pu_norm_pow2 - pf_norm_pow2 + pw_norm_pow2)   / (2* pw_norm_pow2) ;
  double auxiliary_variable_2 =  (pu_norm_pow2 - pf_norm_pow2 + pw_norm_pow2)   / (2 * norm(mP_W, CV_L2)) ;

  p_m = mP_W * auxiliary_variable ;


  //double pm_norm_pow2 = pow(norm(p_m, CV_L2), 2);
  double redundancyCircleRadius 	= sqrt(  (pu_norm_pow2 - pow(auxiliary_variable_2 , 2)) );//page 16 IRINI

  //printf("Redundancy Radius = %f\n", redundancyCircleRadius);


  /* elbow position with respect to the redundancy angle alpha*/
  /* calculate the assosiated spherical angle of the wrist vector mP_W */
  double phi_W = atan2(mP_W.at<double>(2, 0), mP_W.at<double>(1, 0));
  double theta_W = acos(mP_W.at<double>(0, 0) / norm(mP_W, CV_L2));



  Rx(deg2rad(mTaskCoordinates.redundancyAng) , &rotX); //first rotation
  Rz(theta_W , &rotY);	// secodn rotation
  Rx(phi_W   , &rotX2);	//third rotation


  mP_u =   (rotX2 * rotY * rotX * e_z_unit ) * redundancyCircleRadius + p_m;
  //printf("\n\nElbow = \n\n");
  //mP_u.StdOutFormatted();

  /* we calculate the gripper vector perpendicular to the
   * hand vector mP_h and the worlds z-achsis in order to
   * orient the gripper parallel to the table surface as default configuration*/
    mP_h = mP_T - mP_W;
  /* cross product didn't find now in the CvMAT*/

	mP_g.at<double>(0, 0) = mP_h.at<double>(1, 0) * e_z_unit.at<double>(2, 0) - mP_h.at<double>(2, 0) * e_z_unit.at<double>(1, 0);
	mP_g.at<double>(1, 0) = mP_h.at<double>(2, 0) * e_z_unit.at<double>(0, 0) - mP_h.at<double>(0, 0) * e_z_unit.at<double>(2, 0);
	mP_g.at<double>(2, 0) = mP_h.at<double>(0, 0) * e_z_unit.at<double>(1, 0) - mP_h.at<double>(1, 0) * e_z_unit.at<double>(0, 0);

	return(0);
}

double ClosedFormInverseKinematics::CalcTrunkAng()
{
	Mat rotX = Mat::zeros(3, 3, CV_64FC1);
	Mat rotY = Mat::zeros(3, 3, CV_64FC1);
	Mat rotZ = Mat::zeros(3, 3, CV_64FC1);

//	printf("CalcTrunkAng()\n\n");
	/* Endeffector  orientation
	 * we do this due to our own convention of angles determine the
	 * orientation of robots hand. It is not necessary for the overall solution*/
	double phi_eef	 = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(0, 0)) - M_PI_2;
	double theta_eef = deg2rad(mTaskCoordinates.eefOrientationAngle.at<double>(1, 0)) + M_PI;

	Rz(phi_eef,&rotZ);
	Ry(theta_eef,&rotY);

	mP_W = mTaskCoordinates.Pos - rotZ * rotY * mEef;

	//double p_w_length = norm(mP_W, CV_L2);
	double phi_w = atan2(mP_W.at<double>(1, 0), mP_W.at<double>(0, 0));

	if( ( phi_w < deg2rad(-90 )) && (phi_w > deg2rad(-180) )  )
				phi_w = phi_w + deg2rad(360);

  //return(phi_w-deg2rad(90));
  return(phi_w);
}


void ClosedFormInverseKinematics::InverseKinematics()
{
	Mat rotX = Mat::zeros(3, 3, CV_64FC1);
	Mat rotY = Mat::zeros(3, 3, CV_64FC1);
	Mat rotZ = Mat::zeros(3, 3, CV_64FC1);

	TaskCoordinatesToArmGeometry();
	/*Calculate joint angle 1 and 2*/
	mJointAngle.at<double>(1, 0) = atan2(mP_u.at<double>(2, 0), mP_u.at<double>(1, 0));
	mJointAngle.at<double>(2, 0) = acos(mP_u.at<double>(0, 0) / mUperArm.at<double>(0, 0));

  cout << "Angle 1 = " << mJointAngle.at<double>(1, 0) << endl;
  cout << "Angle 2 = " << mJointAngle.at<double>(2, 0) << endl;

/* calculate wrist position */

	/* calculate forearm vector and joint angles 3 and 4*/
	mP_f  =  mP_W - mP_u;

	Rx(-mJointAngle.at<double>(1, 0), &rotX);
	Rz(-mJointAngle.at<double>(2, 0), &rotZ);

	Mat TrafoElbowCS = rotZ * rotX;
	mP_f = TrafoElbowCS * mP_f;
	//printf(" InverseKinematics() \n\n");
	//mP_f.StdOutFormatted();

	mJointAngle.at<double>(3, 0) = atan2(mP_f.at<double>(2, 0), mP_f.at<double>(1, 0));
	mJointAngle.at<double>(4, 0) = acos(mP_f.at<double>(0, 0) / mForeArm.at<double>(0, 0));

	cout << "Angle 3 = " << mJointAngle.at<double>(3, 0) << endl;
	cout << "Angle 4 = " << mJointAngle.at<double>(4, 0) << endl;

	/*calculation of the angles 5 and 6*/
	mP_h = mP_T - mP_W;


	Rx(-mJointAngle.at<double>(3, 0), &rotX);
	Rz(-mJointAngle.at<double>(4, 0), &rotZ);
	Mat TrafoWristCS = rotZ * rotX;

	mP_h =  TrafoWristCS * TrafoElbowCS * mP_h;
	mJointAngle.at<double>(5, 0) = atan2(mP_h.at<double>(2, 0), mP_h.at<double>(1, 0));
	mJointAngle.at<double>(6, 0) = acos(mP_h.at<double>(0, 0) / mEef.at<double>(0, 0));

	cout << "Angle 5 = " << mJointAngle.at<double>(5, 0) << endl;
	cout << "Angle 6 = " << mJointAngle.at<double>(6, 0) << endl;

	/* calculation of the angle 7*/
	Rx(-mJointAngle.at<double>(5, 0), &rotX);
	Rz(-mJointAngle.at<double>(6, 0), &rotZ);
	Mat TrafoEefCS = rotZ * rotX;

	mP_g =  TrafoEefCS * TrafoWristCS * TrafoElbowCS * mP_g;
	mJointAngle.at<double>(7, 0) = atan2(mP_g.at<double>(2, 0), mP_g.at<double>(1, 0));

	cout << "Angle 7 = " << mJointAngle.at<double>(7, 0) << endl;
	return;
}


void ClosedFormInverseKinematics::printMatrix(Mat& mat, const char* name)
{
  cout << name << ":" << endl;

  for(int i = 0; i < mat.size().height; ++i)
  {
    cout << "|\t";

    for(int j = 0; j < mat.size().width; ++j)
    {
      cout << mat.at<double>(i, j) << "\t";
    }

    cout << "|" << endl;
  }

  cout << endl;
  return;
}
