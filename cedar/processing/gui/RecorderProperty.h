/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

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

    File:        Recorder.cpp

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.rub.de
    Date:        2013 08 19

    Description: Header for the @em cedar::aux::gui::RecorderProperty class.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_GUI_RECORDER_PROPERTY_H
#define CEDAR_PROC_GUI_RECORDER_PROPERTY_H

// CEDAR INCLUDES
#include "cedar/processing/gui/namespace.h"
#include "cedar/auxiliaries/Data.h"
#include "cedar/processing/DataSlot.h"

// SYSTEM INCLUDES
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>

/*!@todo Documentation
 */
class cedar::proc::gui::RecorderProperty : public QHBoxLayout
{
  //----------------------------------------------------------------------------
  // macros
  //----------------------------------------------------------------------------
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The public constructor.
  RecorderProperty(const std::string& stepName, cedar::proc::DataSlotPtr slot);

  //!@brief The public destructor. 
  ~RecorderProperty();

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private slots:
  //!@brief Register this slot in the recorder. 
  void registerRecordData(int status);

  //!@brief Informs the recorder that the step size for this slot has changed. 
  void updateStepSize(int value);


  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief the name of the step.
  std::string mStepName;

  //!@brief The name of the slot. 
  std::string mName;

  //!@brief A check box for register and unregister the slot.
  QCheckBox* mCheckBox;

  //!@brief A spin box to set the record interval of the slot.
  QSpinBox* mStepSize; 

  //!@brief The pointer to the data of the slot.
  cedar::aux::ConstDataPtr mData;

  //!@brief Stores the record interval of the slot. Is changed by the mStepSize spin box.
  unsigned int mStepSizeValue;
};

#endif // CEDAR_PROC_GUI_RECORDER_PROPERTY_H