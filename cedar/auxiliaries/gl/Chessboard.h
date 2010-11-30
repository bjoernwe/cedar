/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet-Bochum
                    Institut fuer Neuroinformatik
 
 ----- File:        Chessboard.h
 
 ----- Maintainer:  Hendrik Reimann
 ------Email:       hendrik.reimann@ini.rub.de
 ----- Date:        2010 10 29
 
 ----- Description: visualization for a chessboard
 
 ----- Credits:     
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_AUX_GL_CHESSBOARD_H
#define CEDAR_AUX_GL_CHESSBOARD_H

// LOCAL INCLUDES
#include "namespace.h"
#include "Object.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES

class cedar::aux::gl::Chessboard : public cedar::aux::gl::Object
{
public:
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  /*!@brief standard constructor. 
   * @param name    identifier of the instance, should be unique
   */
	Chessboard(const std::string name);

  /*!@brief constructor. 
   * @param name    identifier of the instance, should be unique
   * @param length    extension of the board in x-direction of the object coordinate frame
   * @param width    extension of the board in y-direction of the object coordinate frame
   * @param height    extension of the board in z-direction of the object coordinate frame
   * @param rows    number of rows in the chessboard
   * @param columns    number of columns in the chessboard
   * @param R1    first color, value for red channel in RGB
   * @param G1    first color, value for green channel in RGB
   * @param B1    first color, value for blue channel in RGB
   * @param R2    second color, value for red channel in RGB
   * @param G2    second color, value for green channel in RGB
   * @param B2    second color, value for blue channel in RGB
   */
	Chessboard(
             const std::string name,
             const double length,
             const double width,
             const double height,
             const int rows,
             const int columns,
             const double R1=1,
             const double G1=1,
             const double B1=1,
             const double R2=0,
             const double G2=0,
             const double B2=0
            );
  
  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief draws a visualization of the object in the current GL context
	void draw();
  
  /*!@brief set extension of the board in x-direction of the object coordinate frame
   * @param value    new extension value
   */
	void setLength(double value);
	
  /*!@brief set extension of the board in y-direction of the object coordinate frame
   * @param value    new extension value
   */
  void setWidth(double value);
	
  /*!@brief set extension of the board in z-direction of the object coordinate frame
   * @param value    new extension value
   */
  void setHeight(double value);
  
  /*!@brief set the number of rows in the chessboard
   * @param value    new extension value
   */
  void setNumberOfRows(int value);
  
  /*!@brief set the number of columns in the chessboard
   * @param value    new extension value
   */
  void setNumberOfColumns(int value);
	
  /*!@brief sets the secondary color of the board, in RGB
   * @param R    value for red channel in RGB color
   * @param G    value for green channel in RGB color
   * @param B    value for blue channel in RGB color
   */
  void setSecondColor(double R, double G, double B);
	
  /*!@brief get extension of the board in x-direction of the object coordinate frame
   * @return    extension value
   */
  double length();
	
  /*!@brief get extension of the board in y-direction of the object coordinate frame
   * @return    extension value
   */
  double width();
	
  /*!@brief get extension of the board in z-direction of the object coordinate frame
   * @return    extension value
   */
  double height();
  
  /*!@brief get the number of rows in the chessboard
   * @return    number of rows
   */
  int numberOfRows();
  
  /*!@brief get the number of columns in the chessboard
   * @return    number of columns
   */
  int numberOfColumns();
  
  //!@brief returns R value of secondary object color in RGB
  double secondColorR();
  
  //!@brief returns G value of secondary object color in RGB
  double secondColorG();
  
  //!@brief returns B value of secondary object color in RGB
  double secondColorB();
  
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:
	double mLength;
  
  double mWidth;
  
  double mHeight;
  
  double mSecondColorR;
  
  double mSecondColorG;
  
  double mSecondColorB;
  
  int mNumberOfRows;
  
  int mNumberOfColumns;
};



#endif // CEDAR_AUX_GL_CHESSBOARD_H
