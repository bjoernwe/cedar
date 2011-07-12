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

    File:        StepItem.cpp

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 11

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/gui/TriggerItem.h"
#include "processing/gui/StepItem.h"
#include "processing/Manager.h"
#include "processing/Data.h"
#include "processing/Trigger.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QPainter>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::TriggerItem::TriggerItem(cedar::proc::TriggerPtr trigger)
:
cedar::proc::gui::GraphicsBase(120, 50,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP
                               | cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_TRIGGER
                               ),
mTrigger (trigger)
{
  this->mClassId = cedar::proc::Manager::getInstance().triggers().getDeclarationOf(trigger);
  this->setFlags(this->flags() | QGraphicsItem::ItemIsSelectable
                               | QGraphicsItem::ItemIsMovable
                               | QGraphicsItem::ItemSendsGeometryChanges
                               );
}

cedar::proc::gui::TriggerItem::~TriggerItem()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


void cedar::proc::gui::TriggerItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * /* event */)
{
}

void cedar::proc::gui::TriggerItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
  painter->save(); // save current painter settings

  QRectF bounds(QPointF(0, 0), QSizeF(this->mWidth, this->mHeight));

  //! @todo make drawing of steps pretty.
  painter->setPen(this->getOutlinePen());

  painter->drawRect(bounds);
  painter->drawText(QPointF(5, 15), this->mClassId->getClassName().c_str());
  painter->drawText(QPointF(5, 25), this->mTrigger->getName().c_str());

  painter->restore(); // restore saved painter settings
}

cedar::proc::TriggerPtr cedar::proc::gui::TriggerItem::getTrigger()
{
  return this->mTrigger;
}

void cedar::proc::gui::TriggerItem::connectTo(cedar::proc::gui::StepItem *pTarget) //!@todo should get a GraphicsBase*
{
  if (!this->getTrigger()->isListener(pTarget->getStep()))
  {
    this->getTrigger()->addListener(pTarget->getStep());
    TriggerConnection *connection = new TriggerConnection(this, pTarget);
    mConnectionInfos.push_back(connection);
  }
}

QVariant cedar::proc::gui::TriggerItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
  switch (change)
  {
    case QGraphicsItem::ItemPositionHasChanged:
      for (size_t i = 0; i < this->mConnectionInfos.size(); ++i)
      {
        this->mConnectionInfos.at(i)->update();
      }
      break;

    default:
      break;
  }
  return QGraphicsItem::itemChange(change, value);
}
