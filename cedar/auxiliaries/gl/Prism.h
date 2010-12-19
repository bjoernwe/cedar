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
 
 ----- File:        Prism.h
 
 ----- Maintainer:  Hendrik Reimann
 ------Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 10 29
 
 ----- Description: visualization for a prism with equilateral triangular base
 
 ----- Credits:     
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_AUX_GL_PRISM_H
#define CEDAR_AUX_GL_PRISM_H

// LOCAL INCLUDES
#include "namespace.h"
#include "Object.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

/*!@brief Simple OpenGL visualization of a prism with a equilateral triangle as base
 *
 * This class visualizes an instance of cedar::aux::Object as a prism with specified dimensions
 *
 * @remarks To get a simple visualization of the Object on screen, add an instance of this class to a
 * cedar::aux::gl::Scene and create a cedar::aux::gui::Viewer for it
 */
class cedar::aux::gl::Prism : public cedar::aux::gl::Object
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  /*!@brief standard constructor. 
   * @param name    identifier of the instance, should be unique
   */
	Prism(cedar::aux::ObjectPtr pObject);

  /*!@brief constructor. 
   * @param name    identifier of the instance, should be unique
   * @param width    edge length of the prism's triangular base
   * @param height    height of the prism, i.e. distance between floor (=base) and ceiling
   * @param R    color, value for red channel in RGB
   * @param G    color, value for green channel in RGB
   * @param B    color, value for blue channel in RGB
   */
  Prism(
         cedar::aux::ObjectPtr pObject,
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

  /*!@brief set edge length of the prism's triangular base
   * @param value    new edge length value
   */
	void setWidth(double value);
	
  /*!@brief set height of the prism, i.e. distance between floor (=base) and ceiling
   * @param value    new height value
   */
  void setHeight(double value);
	
  /*!@brief get edge length of the prism's triangular base
   * @return    edge length value
   */
  double width();
	
  /*!@brief get height of the prism, i.e. distance between floor (=base) and ceiling
   * @return    height value
   */
  double height();
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:
  double mWidth;
  
  double mHeight;
};

#endif // CEDAR_AUX_GL_PRISM_H
