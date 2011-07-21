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

 ----- Author:      
 ----- Email:       
 ----- Date:        2010 12 04

 ----- Description: Implements all unit tests for the @em cedar::aux::net

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

// LOCAL INCLUDES
#include "auxiliaries/net/NetBlockingReader.h"
#include "auxiliaries/net/NetReader.h"
#include "auxiliaries/net/NetWriter.h"
#include "auxiliaries/net/exceptions/NetException.h"

// PROJECT INCLUDES
#include "auxiliaries/LogFile.h"

// SYSTEM INCLUDES
#include <iostream>
#include <string>
#include <sstream>

using namespace cedar::aux::net;
using namespace std;



int main()
{
  cedar::aux::LogFile log_file("net.log");

  log_file.addTimeStamp();
  log_file << std::endl;

  // the number of errors encountered in this test
  signed char errors = 0;
  signed char mat_errors = 0;
  
  //// Test for simple variable passing ////
  try
  {
    float fl;

#define MYSIMPLEPORT "CEDAR-UNIT-TEST-SIMPLE"
    NetWriter<float> myFloatWriter(MYSIMPLEPORT);
    NetBlockingReader<float> myFloatReader(MYSIMPLEPORT);

    fl= 42.42;
    myFloatWriter.write(fl);

    if (myFloatReader.read() != fl)
    {
      mat_errors++;
    }
  }
  catch (cedar::aux::exc::ExceptionBase &E)
  {
    log_file << E.exceptionInfo() << std::endl;
    return 1;
  }

  if (mat_errors)
  {
    errors++;
    log_file << "ERROR with float" << std::endl;
    mat_errors= 0;
  }
  else
  {
    log_file << "OK (float)" << std::endl;
  }

  //// cv::Mat Test ////
  typedef double TestType;
#define SIZE 5000
#define MYPORT "CEDAR-UNIT-TEST"

#if 1
  // preparing ...
  cv::Mat mat= cv::Mat::eye(SIZE, 2, CV_64F);
  cv::Mat mat2= cv::Mat::zeros(SIZE, 2, CV_64F);
  cv::Mat mat3= cv::Mat::zeros(16, 2, CV_64F);

  for (int i=0; i < SIZE; i++)
  {
    mat.at<TestType>(i,0)= 300 + i;
    mat.at<TestType>(i,1)= SIZE + 500 + i;

    mat2.at<TestType>(i,0)= -1;
    mat2.at<TestType>(i,1)= -1;
  }

  try
  {
    // writing ...
    NetWriter<cv::Mat> myMatWriter(MYPORT);

    // ... reading
    NetBlockingReader<cv::Mat> myMatReader2(MYPORT);
    NetBlockingReader<cv::Mat> myMatReader3(MYPORT);

    myMatWriter.write(mat);
    mat2= myMatReader2.read();
    mat3= myMatReader3.read();

    // test it
    for (int i=0; i < SIZE; i++)
    {
      if (mat2.at<TestType>(i,0) != 300 + i
          || mat2.at<TestType>(i,1) != SIZE + 500 +i
          // auch mat3 testen ...
          || mat3.at<TestType>(i,0) != 300 + i
          || mat3.at<TestType>(i,1) != SIZE + 500 +i)
      {
        // FAIL
        mat_errors++;
      }
    }

  } // end try
  catch (cedar::aux::exc::ExceptionBase &E)
  {
    log_file << E.exceptionInfo() << std::endl;
    return 1;
  }

  if (mat_errors)
  {
    log_file << "ERROR with cv::Mat simple write() / read()" << std::endl;
    errors++;
    mat_errors= 0;
  }
  else
  {
    log_file << "OK (cv::Mat)" << std::endl;
  }

#endif

#if 1
  //// cv::Mat_ (note the underscore!) Test ////
  try
  {
    typedef cv::Mat_<float> floatMat_;
    floatMat_ mat4, mat5;
  
    mat_errors= 0;
    NetWriter< cv::Mat_<float> > myMatWriter2_1(MYPORT"_");
    NetBlockingReader< cv::Mat_<float> > myMatReader2_1(MYPORT"_");

    mat4= floatMat_(2,2);


    for (int i = 0; i < mat4.rows; i++)
    {
      for(int j = 0; j < mat4.cols; j++)
      {
              mat4(i,j)= 79;
      }
   }

    myMatWriter2_1.write( mat4 );
    mat5= myMatReader2_1.read(); 


    if (mat5(1,1) != mat4(1,1))
    {
      mat_errors++;
      log_file << "ERROR with cv::Mat_ test" << std::endl;
    }
  }
  catch (cedar::aux::exc::NetWaitingForWriterException &E)
  {
    log_file << "ERROR writer not initialized yet, should wait" << std::endl;
    return 1;
  }
  catch (cedar::aux::exc::NetMissingRessourceException &E)
  {
    log_file << "ERROR YARP NAME SERVER NOT RUNNING" << std::endl;
    return 1;
  }
  catch (cedar::aux::exc::NetUnexpectedDataException &E)
  {
    log_file << "ERROR YARP unexpected data" << std::endl;
    return 1;
  }
#endif
 
  if (mat_errors)
  {
    log_file << "ERROR with cv::Mat_ simple" << std::endl;
    errors++;
    mat_errors= 0;
  }
  else
  {
    log_file << "OK (cv::Mat_)" << std::endl;
  }

std::cout << "done" << std::endl;

  return errors;
}

