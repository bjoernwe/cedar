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

    File:        DataSlotItem.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 15

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_GUI_DATA_SLOT_ITEM_H
#define CEDAR_PROC_GUI_DATA_SLOT_ITEM_H

// LOCAL INCLUDES
#include "processing/gui/namespace.h"
#include "processing/gui/GraphicsBase.h"
#include "auxiliaries/Data.h"
#include "processing/DataRole.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES


/*!@brief Abstract description of the class.
 *
 * More detailed description of the class.
 */
class cedar::proc::gui::DataSlotItem : public cedar::proc::gui::GraphicsBase
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  DataSlotItem(cedar::proc::gui::StepItem *pParent,
               cedar::aux::DataPtr data,
               const std::string& dataName,
               cedar::proc::DataRole::Id role);

  //!@brief Destructor
  ~DataSlotItem();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);

  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

  void connectTo(cedar::proc::gui::DataSlotItem *pTarget);

  void disconnect();

  const std::string& getName() const;

  bool canConnect() const;
  bool canConnectTo(GraphicsBase* pTarget) const;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  cedar::proc::gui::StepItem *mpStep;
  cedar::aux::DataPtr mData;
  cedar::proc::DataRole::Id mRole;
  const std::string& mDataName;
  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:

}; // class DataSlotItem

#endif // CEDAR_PROC_GUI_DATA_SLOT_ITEM_H

