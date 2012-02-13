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

    File:        KukaArm.cpp

    Maintainer:  Hendrik Reimann
    Email:       hendrik.reimann@ini.rub.de
    Date:        2010 12 14

    Description: implementation for a class visualizing the arm of CoRA

    Credits:

======================================================================================================================*/

#define NOMINMAX // to avoid Windows issues
#define GL_GLEXT_PROTOTYPES // to avoid a problem with finding some GL stuff, apparently caused by Qt
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// CEDAR INCLUDES
#include "cedar/devices/robot/gl/namespace.h"
#include "cedar/devices/robot/gl/KukaArm.h"
#include "cedar/auxiliaries/gl/drawShapes.h"
#include "cedar/auxiliaries/gl/gl.h"
#include "cedar/auxiliaries/gl/glu.h"

// SYSTEM INCLUDES
 #include <QTextStream>
 #include <QFile>

// define constants
const float cedar::dev::robot::gl::KukaArm::mNoSpecular[3] = {0.0f, 0.0f, 0.0f};
const float cedar::dev::robot::gl::KukaArm::mSegment_Ambient[3] = {0.0f, 0.0f, 0.0f};
const float cedar::dev::robot::gl::KukaArm::mSegment_Diffuse[3] = {1.0f, 0.39215699f, 0.0f};
const float cedar::dev::robot::gl::KukaArm::mSegment_Specular[3] = {0.40000001f, 0.16078401f, 0.0f};
const float cedar::dev::robot::gl::KukaArm::mSegment_Shininess[1] = {1.0f};
const float cedar::dev::robot::gl::KukaArm::mRing_Ambient[3] = {0.0f, 0.0f, 0.0f};
const float cedar::dev::robot::gl::KukaArm::mRing_Diffuse[3] = {0.17647099f, 0.17647099f, 0.17647099f};
const float cedar::dev::robot::gl::KukaArm::mRing_Specular[3] = {0.372549f, 0.372549f, 0.372549f};
const float cedar::dev::robot::gl::KukaArm::mRing_Shininess[1] = {0.42631999f};


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::dev::robot::gl::KukaArm::KukaArm
(
  cedar::dev::robot::KinematicChainModelPtr& rpKinematicChainModel,
  const std::string& pathToPolygonData
)
:
cedar::dev::robot::gl::KinematicChain(rpKinematicChainModel)
{
  loadPolygonData(pathToPolygonData);
}

cedar::dev::robot::gl::KukaArm::~KukaArm()
{

}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::dev::robot::gl::KukaArm::initializeGl()
{
  std::cout << "initializing resources for KUKA LBR4 visualization" << std::endl;
  // base segment
  glGenBuffers(1, &mBaseSegmentVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mBaseSegmentVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mBaseSegmentVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mBaseSegmentVertexNumber, mBaseSegmentVertex);
  glGenBuffers(1, &mBaseSegmentIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBaseSegmentIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBaseSegmentFacesNumber*3 * sizeof(GLushort), mBaseSegmentIndex, GL_STATIC_DRAW);

  // base ring
  glGenBuffers(1, &mBaseRingVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mBaseRingVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mBaseRingVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mBaseRingVertexNumber, mBaseRingVertex);
  glGenBuffers(1, &mBaseRingIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBaseRingIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mBaseRingFacesNumber*3 * sizeof(GLushort), mBaseRingIndex, GL_STATIC_DRAW);

  // forward segment
  glGenBuffers(1, &mForwardSegmentVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mForwardSegmentVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mForwardSegmentVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mForwardSegmentVertexNumber, mForwardSegmentVertex);
  glGenBuffers(1, &mForwardSegmentIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mForwardSegmentIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mForwardSegmentFacesNumber*3 * sizeof(GLushort), mForwardSegmentIndex, GL_STATIC_DRAW);

  // forward ring
  glGenBuffers(1, &mForwardRingVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mForwardRingVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mForwardRingVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mForwardRingVertexNumber, mForwardRingVertex);
  glGenBuffers(1, &mForwardRingIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mForwardRingIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mForwardRingFacesNumber*3 * sizeof(GLushort), mForwardRingIndex, GL_STATIC_DRAW);

  // inverse segment
  glGenBuffers(1, &mInverseSegmentVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mInverseSegmentVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mInverseSegmentVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mInverseSegmentVertexNumber, mInverseSegmentVertex);
  glGenBuffers(1, &mInverseSegmentIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mInverseSegmentIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mInverseSegmentFacesNumber*3 * sizeof(GLushort), mInverseSegmentIndex, GL_STATIC_DRAW);

  // inverse ring
  glGenBuffers(1, &mInverseRingVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mInverseRingVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mInverseRingVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mInverseRingVertexNumber, mInverseRingVertex);
  glGenBuffers(1, &mInverseRingIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mInverseRingIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mInverseRingFacesNumber*3 * sizeof(GLushort), mInverseRingIndex, GL_STATIC_DRAW);

  // wrist segment
  glGenBuffers(1, &mWristSegmentVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mWristSegmentVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mWristSegmentVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mWristSegmentVertexNumber, mWristSegmentVertex);
  glGenBuffers(1, &mWristSegmentIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWristSegmentIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mWristSegmentFacesNumber*3 * sizeof(GLushort), mWristSegmentIndex, GL_STATIC_DRAW);

  // wrist sphere
  glGenBuffers(1, &mWristSphereVertexVboId);
  glBindBuffer(GL_ARRAY_BUFFER, mWristSphereVertexVboId);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mWristSphereVertexNumber, NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * mWristSphereVertexNumber, mWristSphereVertex);
  glGenBuffers(1, &mWristSphereIndexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mWristSphereIndexVboId);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mWristSphereFacesNumber*3 * sizeof(GLushort), mWristSphereIndex, GL_STATIC_DRAW);

}

void cedar::dev::robot::gl::KukaArm::drawBase()
{
  prepareDraw();
  setMaterial(SEGMENT);
  this->drawElement(mBaseSegmentVertexVboId, mBaseSegmentIndexVboId, mBaseSegmentFacesNumber);
  setMaterial(RING);
  this->drawElement(mBaseRingVertexVboId, mBaseRingIndexVboId, mBaseRingFacesNumber);
  setMaterial(NO_MATERIAL);
}

void cedar::dev::robot::gl::KukaArm::drawSegment(unsigned int index)
{
  // move to origin transformation and resave it to the stack
  glPopMatrix();
  glPushMatrix();

  // move to object coordinates
  mTransformationTranspose = mpKinematicChainModel->getJointTransformation(index).t();
  glMultMatrixd((GLdouble*)mTransformationTranspose.data);

  if (isDrawingLocalCoordinateFrame())
  {
    cedar::aux::gl::drawAxes(0.2);
  }
  switch (index)
  {
  case 0:
    setMaterial(SEGMENT);
    this->drawElement(mForwardSegmentVertexVboId, mForwardSegmentIndexVboId, mForwardSegmentFacesNumber);
    setMaterial(RING);
    this->drawElement(mForwardRingVertexVboId, mForwardRingIndexVboId, mForwardRingFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  case 1:
    glRotated(90.0, 1.0, 0.0, 0.0);
    setMaterial(SEGMENT);
    this->drawElement(mInverseSegmentVertexVboId, mInverseSegmentIndexVboId, mInverseSegmentFacesNumber);
    setMaterial(RING);
    this->drawElement(mInverseRingVertexVboId, mInverseRingIndexVboId, mInverseRingFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  case 2:
    glRotated(180.0, 0.0, 0.0, 1.0);
    setMaterial(SEGMENT);
    this->drawElement(mForwardSegmentVertexVboId, mForwardSegmentIndexVboId, mForwardSegmentFacesNumber);
    setMaterial(RING);
    this->drawElement(mForwardRingVertexVboId, mForwardRingIndexVboId, mForwardRingFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  case 3:
    glRotated(90.0, 1.0, 0.0, 0.0);
    glRotated(180.0, 0.0, 1.0, 0.0);
    setMaterial(SEGMENT);
    this->drawElement(mInverseSegmentVertexVboId, mInverseSegmentIndexVboId, mInverseSegmentFacesNumber);
    setMaterial(RING);
    this->drawElement(mInverseRingVertexVboId, mInverseRingIndexVboId, mInverseRingFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  case 4:
    setMaterial(SEGMENT);
    this->drawElement(mWristSegmentVertexVboId, mWristSegmentIndexVboId, mWristSegmentFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  case 5:
    glRotated(90.0, 1.0, 0.0, 0.0);
    setMaterial(SEGMENT);
    this->drawElement(mWristSphereVertexVboId, mWristSphereIndexVboId, mWristSphereFacesNumber);
    setMaterial(NO_MATERIAL);
    break;
  }
}

void cedar::dev::robot::gl::KukaArm::drawEndEffector()
{

}

void cedar::dev::robot::gl::KukaArm::drawElement(const GLuint vertexVboId, const GLuint indexVboId, const unsigned int numberOfFaces)
{
  // bind the buffers
  glBindBuffer(GL_ARRAY_BUFFER, vertexVboId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVboId);

  // set the pointers
  glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
  glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(20));
  glColorPointer(4, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(32));
  glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));

  // prepare
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);

  // draw
  glDrawElements(GL_TRIANGLES, numberOfFaces*3, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

  // reset
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void cedar::dev::robot::gl::KukaArm::setMaterial(int material)
{
  switch (material)
  {
  case NO_MATERIAL:
    glMaterialfv(GL_FRONT, GL_SPECULAR, mNoSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mNoSpecular); //todo: this is probably an error
    glEnable(GL_COLOR_MATERIAL);
    break;
  case SEGMENT:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mSegment_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mSegment_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mSegment_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mSegment_Shininess);
    break;
  case RING:
    glDisable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mRing_Ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mRing_Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mRing_Specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mRing_Shininess);
    break;
  }
}

void cedar::dev::robot::gl::KukaArm::loadPolygonData(const std::string& pathToPolygonData)
{
  QString base_segment_data_file_name = QString(pathToPolygonData.c_str()) + QString("base_segment.ply");
  loadBaseSegmentData(base_segment_data_file_name);

  QString base_ring_data_file_name = QString(pathToPolygonData.c_str()) + QString("base_ring.ply");
  loadBaseRingData(base_ring_data_file_name);

  QString forward_segment_data_file_name = QString(pathToPolygonData.c_str()) + QString("forward_segment.ply");
  loadForwardSegmentData(forward_segment_data_file_name);

  QString forward_ring_data_file_name = QString(pathToPolygonData.c_str()) + QString("forward_ring.ply");
  loadForwardRingData(forward_ring_data_file_name);

  QString inverse_segment_data_file_name = QString(pathToPolygonData.c_str()) + QString("inverse_segment.ply");
  loadInverseSegmentData(inverse_segment_data_file_name);

  QString inverse_ring_data_file_name = QString(pathToPolygonData.c_str()) + QString("inverse_ring.ply");
  loadInverseRingData(inverse_ring_data_file_name);

  QString wrist_segment_data_file_name = QString(pathToPolygonData.c_str()) + QString("wrist_segment.ply");
  loadWristSegmentData(wrist_segment_data_file_name);

  QString wrist_sphere_data_file_name = QString(pathToPolygonData.c_str()) + QString("wrist_sphere.ply");
  loadWristSphereData(wrist_sphere_data_file_name);

}

void cedar::dev::robot::gl::KukaArm::loadBaseSegmentData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mBaseSegmentVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentVertex[i].normal[2] = number.toFloat();
    }


    // read index data
    for (unsigned int i=0; i<mBaseSegmentFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mBaseSegmentIndex[3*i+2] = static_cast<GLushort>(number.toInt());


    }
  }
  else
  {
    std::cout << "could not read polygon file for base" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mBaseSegmentVertexNumber; i++)
  {
    mBaseSegmentVertex[i].colour[0] = 1.0;
    mBaseSegmentVertex[i].colour[1] = 0.5;
    mBaseSegmentVertex[i].colour[2] = 0.0;
    mBaseSegmentVertex[i].colour[3] = 1.0;
  }
//
//  std::cout << "Base: " << std::endl;
//
//  std::cout << "first vertex: " << std::endl;
//  std::cout << mBaseSegmentVertex[0].location[0] << " ";
//  std::cout << mBaseSegmentVertex[0].location[1] << " ";
//  std::cout << mBaseSegmentVertex[0].location[2] << " ";
//  std::cout << mBaseSegmentVertex[0].normal[0] << " ";
//  std::cout << mBaseSegmentVertex[0].normal[1] << " ";
//  std::cout << mBaseSegmentVertex[0].normal[2] << std::endl;
//
//  std::cout << "last vertex: " << std::endl;
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].location[0] << " ";
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].location[1] << " ";
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].location[2] << " ";
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].normal[0] << " ";
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].normal[1] << " ";
//  std::cout << mBaseSegmentVertex[mBaseSegmentVertexNumber-1].normal[2] << std::endl;
//
//  std::cout << "first indices: " << std::endl;
//  std::cout << static_cast<int>(mBaseSegmentIndex[0]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[1]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[2]) << std::endl;
//  std::cout << static_cast<int>(mBaseSegmentIndex[3]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[4]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[5]) << std::endl;
//
//  std::cout << "last indices: " << std::endl;
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-2)*3]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-2)*3+1]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-2)*3+2]) << std::endl;
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-1)*3]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-1)*3+1]) << " ";
//  std::cout << static_cast<int>(mBaseSegmentIndex[(mBaseSegmentFacesNumber-1)*3+2]) << std::endl;
//
//  std::cout << std::endl;
}

void cedar::dev::robot::gl::KukaArm::loadBaseRingData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mBaseRingVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingVertex[i].normal[2] = number.toFloat();
    }


    // read index data
    for (unsigned int i=0; i<mBaseRingFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mBaseRingIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mBaseRingIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read polygon file for base ring" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mBaseRingVertexNumber; i++)
  {
    mBaseRingVertex[i].colour[0] = 1.0;
    mBaseRingVertex[i].colour[1] = 1.0;
    mBaseRingVertex[i].colour[2] = 1.0;
    mBaseRingVertex[i].colour[3] = 1.0;
  }

//  std::cout << "Base ring: " << std::endl;
//
//  std::cout << "first vertex: " << std::endl;
//  std::cout << mBaseRingVertex[0].location[0] << " ";
//  std::cout << mBaseRingVertex[0].location[1] << " ";
//  std::cout << mBaseRingVertex[0].location[2] << " ";
//  std::cout << mBaseRingVertex[0].normal[0] << " ";
//  std::cout << mBaseRingVertex[0].normal[1] << " ";
//  std::cout << mBaseRingVertex[0].normal[2] << std::endl;
//
//  std::cout << "last vertex: " << std::endl;
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].location[0] << " ";
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].location[1] << " ";
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].location[2] << " ";
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].normal[0] << " ";
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].normal[1] << " ";
//  std::cout << mBaseRingVertex[mBaseRingVertexNumber-1].normal[2] << std::endl;
//
//  std::cout << "first indices: " << std::endl;
//  std::cout << static_cast<int>(mBaseRingIndex[0]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[1]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[2]) << std::endl;
//  std::cout << static_cast<int>(mBaseRingIndex[3]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[4]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[5]) << std::endl;
//
//  std::cout << "last indices: " << std::endl;
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-2)*3]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-2)*3+1]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-2)*3+2]) << std::endl;
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-1)*3]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-1)*3+1]) << " ";
//  std::cout << static_cast<int>(mBaseRingIndex[(mBaseRingFacesNumber-1)*3+2]) << std::endl;
//
//  std::cout << std::endl;
}

void cedar::dev::robot::gl::KukaArm::loadForwardSegmentData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mForwardSegmentVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentVertex[i].normal[2] = number.toFloat();
    }

    // read index data
    for (unsigned int i=0; i<mForwardSegmentFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mForwardSegmentIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file forward_segment.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mForwardSegmentVertexNumber; i++)
  {
    mForwardSegmentVertex[i].colour[0] = 1.0;
    mForwardSegmentVertex[i].colour[1] = 0.5;
    mForwardSegmentVertex[i].colour[2] = 0.0;
    mForwardSegmentVertex[i].colour[3] = 1.0;
  }
}

void cedar::dev::robot::gl::KukaArm::loadForwardRingData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mForwardRingVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingVertex[i].normal[2] = number.toFloat();
    }


    // read index data
    for (unsigned int i=0; i<mForwardRingFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mForwardRingIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mForwardRingIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file forward_ring.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mForwardRingVertexNumber; i++)
  {
    mForwardRingVertex[i].colour[0] = 1.0;
    mForwardRingVertex[i].colour[1] = 1.0;
    mForwardRingVertex[i].colour[2] = 1.0;
    mForwardRingVertex[i].colour[3] = 1.0;
  }
}

void cedar::dev::robot::gl::KukaArm::loadInverseSegmentData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mInverseSegmentVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentVertex[i].normal[2] = number.toFloat();
    }

    // read index data
    for (unsigned int i=0; i<mInverseSegmentFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mInverseSegmentIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file forward_segment.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mInverseSegmentVertexNumber; i++)
  {
    mInverseSegmentVertex[i].colour[0] = 1.0;
    mInverseSegmentVertex[i].colour[1] = 0.5;
    mInverseSegmentVertex[i].colour[2] = 0.0;
    mInverseSegmentVertex[i].colour[3] = 1.0;
  }
}

void cedar::dev::robot::gl::KukaArm::loadInverseRingData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mInverseRingVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingVertex[i].normal[2] = number.toFloat();
    }


    // read index data
    for (unsigned int i=0; i<mInverseRingFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mInverseRingIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mInverseRingIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file forward_ring.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mInverseRingVertexNumber; i++)
  {
    mInverseRingVertex[i].colour[0] = 1.0;
    mInverseRingVertex[i].colour[1] = 1.0;
    mInverseRingVertex[i].colour[2] = 1.0;
    mInverseRingVertex[i].colour[3] = 1.0;
  }
}

void cedar::dev::robot::gl::KukaArm::loadWristSegmentData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mWristSegmentVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentVertex[i].normal[2] = number.toFloat();
    }

    // read index data
    for (unsigned int i=0; i<mWristSegmentFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mWristSegmentIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file wrist_segment.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mWristSegmentVertexNumber; i++)
  {
    mWristSegmentVertex[i].colour[0] = 1.0;
    mWristSegmentVertex[i].colour[1] = 0.5;
    mWristSegmentVertex[i].colour[2] = 0.0;
    mWristSegmentVertex[i].colour[3] = 1.0;
  }
}

void cedar::dev::robot::gl::KukaArm::loadWristSphereData(const QString& dataFile)
{
  QFile data(dataFile);
  if (data.open(QFile::ReadOnly))
  {
    QTextStream text_stream(&data);
    QString line;
    QTextStream line_stream;
    QString number;
    float scale = 0.001; // mm -> m

    // read header
    for (unsigned int i=0; i<13; i++)
    {
      line = text_stream.readLine();
    }

    // read vertex data
    for (unsigned int i=0; i<mWristSphereVertexNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // position x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].location[0] = number.toFloat() * scale;

      // position y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].location[1] = number.toFloat() * scale;

      // position z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].location[2] = number.toFloat() * scale;

      // normal x
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].normal[0] = number.toFloat();

      // normal y
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].normal[1] = number.toFloat();

      // normal z
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereVertex[i].normal[2] = number.toFloat();
    }

    // read index data
    for (unsigned int i=0; i<mWristSphereFacesNumber; i++)
    {
      line = text_stream.readLine();
      QTextStream line_stream(&line);

      // the leading "3"
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }

      // first index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereIndex[3*i] = static_cast<GLushort>(number.toInt());

      // second index
      number = "";
      while (!number.endsWith(" "))
      {
        number.append(line_stream.read(1));
      }
      mWristSphereIndex[3*i+1] = static_cast<GLushort>(number.toInt());

      // third index
      number = "";
      while (!line_stream.atEnd())
      {
        number.append(line_stream.read(1));
      }
      mWristSphereIndex[3*i+2] = static_cast<GLushort>(number.toInt());
    }
  }
  else
  {
    std::cout << "could not read file wrist_segment.ply" << std::endl;
  }

  // Colors
  for (unsigned int i = 0; i < mWristSphereVertexNumber; i++)
  {
    mWristSphereVertex[i].colour[0] = 1.0;
    mWristSphereVertex[i].colour[1] = 0.5;
    mWristSphereVertex[i].colour[2] = 0.0;
    mWristSphereVertex[i].colour[3] = 1.0;
  }
}
