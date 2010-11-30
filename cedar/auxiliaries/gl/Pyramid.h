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

 ----- Institute:   Ruhr-Universitaet-Bochum
                    Institut fuer Neuroinformatik
 
 ----- File:        Pyramid.h
 
 ----- Maintainer:  Hendrik Reimann
 ------Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 10 29
 
 ----- Description: visualization for a pyramid
 
 ----- Credits:     
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_AUX_GL_PYRAMID_H
#define CEDAR_AUX_GL_PYRAMID_H

// LOCAL INCLUDES
#include "namespace.h"
#include "Object.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

class cedar::aux::gl::Pyramid : public cedar::aux::gl::Object
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  /*!@brief standard constructor. 
   * @param name    identifier of the instance, should be unique
   */
	Pyramid(const std::string name);

  /*!@brief constructor. 
   * @param name    identifier of the instance, should be unique
   * @param length    extension of the pyramid's base in x-direction of object coordinate frame
   * @param width    extension of the pyramid's base in y-direction of object coordinate frame
   * @param height    height of the pyramid, i.e. distance from base to tip
   * @param R    color, value for red channel in RGB
   * @param G    color, value for green channel in RGB
   * @param B    color, value for blue channel in RGB
   */
	Pyramid(
          const std::string name,
          const double length,
          const double width,
          const double height,
          const double R=1,
          const double G=0,
          const double B=0
         );
  
  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief draws a visualization of the object in the current GL context
	void draw();

  /*!@brief set extension of the pyramid's base in x-direction of the object coordinate frame
   * @param value    new extension value
   */
	void setLength(double value);
	
  /*!@brief set extension of the pyramid's base in y-direction of the object coordinate frame
   * @param value    new extension value
   */
  void setWidth(double value);
	
  /*!@brief set height of the pyramid, i.e. the distance from the base to the tip
   * @param value    new height value
   */
  void setHeight(double value);
	
  /*!@brief get extension of the pyramid's base in x-direction of the object coordinate frame
   * @return    extension value
   */
  double length();
	
  /*!@brief get extension of the pyramid's base in y-direction of the object coordinate frame
   * @return    extension value
   */
  double width();
	
  /*!@brief get height of the pyramid, i.e. the distance from the base to the tip
   * @return    height value
   */
  double height();
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:
  double mLength;
  
  double mWidth;
  
  double mHeight;
};



#endif // CEDAR_AUX_GL_PYRAMID_H
