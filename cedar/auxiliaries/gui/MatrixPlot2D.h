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

    File:        MatrixPlot.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 14

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_GUI_MATRIX_PLOT_2D_H
#define CEDAR_AUX_GUI_MATRIX_PLOT_2D_H

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/namespace.h"
#include "cedar/auxiliaries/gui/PlotInterface.h"

// SYSTEM INCLUDES
#include <QWidget>
#include <QReadWriteLock>
#include <opencv2/opencv.hpp>
#include <qwtplot3d/qwt3d_gridplot.h>
#include <qwtplot3d/qwt3d_function.h>
#include <qwtplot3d/qwt3d_plot3d.h>
#include <qwtplot3d/qwt3d_io.h>

/*!@brief Matrix plot that can display 2D matrices (i.e. vectors).
 *
 * @todo Write more detailed description of the class here.
 */
class cedar::aux::gui::MatrixPlot2D : public PlotInterface
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
private:
  class Perspective
  {
    public:
      Perspective(const std::string& name = "perspective",
                  double rotationX = 0, double rotationY = 0, double rotationZ = 0,
                  double scaleX = 1, double scaleY = 1, double scaleZ = 1,
                  double shiftX = 0, double shiftY = 0, double shiftZ = 0,
                  double zoom = 1);

      void applyTo(Qwt3D::Plot3D* pPlot);

      const std::string& getName() const
      {
        return this->mName;
      }

    private:
      std::string mName;
      double mRotation[3];
      double mScale[3];
      double mShift[3];
      double mZoom;
  };

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  MatrixPlot2D(QWidget *pParent = NULL);

  //!@brief Constructor expecting a DataPtr.
  MatrixPlot2D(cedar::aux::DataPtr matData, const std::string& title, QWidget *pParent = NULL);

  //!@brief Destructor
  ~MatrixPlot2D();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief display data
  void plot(cedar::aux::DataPtr matData, const std::string& title);
  //!@brief show or hide the plot grid
  void showGrid(bool show);
  //!@brief handle timer events
  void timerEvent(QTimerEvent *pEvent);

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  //!@brief create and handle the context menu
  void contextMenuEvent(QContextMenuEvent * pEvent);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief initialize
  void init();

  //!@brief reset the perspective of the plot
  void resetPerspective(size_t perspectiveIndex = 0);

  //!@brief delete the allocated array data
  void deleteArrayData();

  //!@brief update the allocated array data
  void updateArrayData();

  //!@brief Applies the labels from the data object to the plot.
  void applyLabels();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //!@brief the displayed MatData
  cedar::aux::MatDataPtr mMatData;
  //!@brief flag if plot grid should be displayed
  bool mShowGridLines;
  //!@biref the plot object
  Qwt3D::GridPlot *mpPlot;
  //!@brief vector of possible perspectives
  std::vector<Perspective> mPerspectives;
  //!@brief row count of data
  size_t mDataRows;
  //!@brief column count of data
  size_t mDataCols;
  //!@brief 2D array data
  Qwt3D::Triple** mppArrayData;
}; // class cedar::aux::gui::MatrixPlot2D

#endif // CEDAR_AUX_GUI_MATRIX_PLOT_2D_H
