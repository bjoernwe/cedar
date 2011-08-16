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
    Date:        2011 05 27

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/gui/StepItem.h"
#include "processing/gui/DataPlotter.h"
#include "processing/gui/DataSlotItem.h"
#include "processing/gui/TriggerItem.h"
#include "processing/gui/exceptions.h"
#include "processing/DataSlot.h"
#include "processing/Manager.h"
#include "auxiliaries/Data.h"
#include "processing/Step.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QPainter>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QGraphicsDropShadowEffect>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::StepItem::StepItem(cedar::proc::StepPtr step, QMainWindow* pMainWindow)
:
cedar::proc::gui::GraphicsBase(160, 50,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_NONE),
mpMainWindow(pMainWindow),
mStepIcon(":/steps/no_icon.svg")
{
  this->setStep(step);
  
  this->construct();
}

cedar::proc::gui::StepItem::StepItem(QMainWindow* pMainWindow)
:
cedar::proc::gui::GraphicsBase(160, 50,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_STEP,
                               cedar::proc::gui::GraphicsBase::GRAPHICS_GROUP_NONE),
mpMainWindow(pMainWindow),
mStepIcon(":/steps/no_icon.svg")
{
  this->construct();
}

void cedar::proc::gui::StepItem::construct()
{
  this->setFlags(this->flags() | QGraphicsItem::ItemIsSelectable
                               | QGraphicsItem::ItemIsMovable
                               );

  QGraphicsDropShadowEffect *p_effect = new QGraphicsDropShadowEffect();
  p_effect->setBlurRadius(5.0);
  p_effect->setOffset(3.0, 3.0);
  this->setGraphicsEffect(p_effect);
}

cedar::proc::gui::StepItem::~StepItem()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::gui::StepItem::stepStateChanged()
{
  switch (this->mStep->getState())
  {
    case cedar::proc::Step::STATE_EXCEPTION:
    case cedar::proc::Step::STATE_NOT_RUNNING:
      this->setOutlineColor(Qt::darkGray);
      this->setFillColor(QColor(235, 235, 235));
      break;

    default:
      this->setOutlineColor(cedar::proc::gui::GraphicsBase::mDefaultOutlineColor);
      this->setFillColor(cedar::proc::gui::GraphicsBase::mDefaultFillColor);
  }
  // append <font>s to make this a rich text, thus automatically word-wrapping the tool tip.
  const std::string& annotation = this->mStep->getStateAnnotation();
  QString tool_tip = "";
  if (!annotation.empty())
  {
    tool_tip = QString("<FONT COLOR=BLACK>") + annotation.c_str() + QString("</FONT>");
  }
  this->setToolTip(tool_tip);
  this->update();
}

void cedar::proc::gui::StepItem::setStep(cedar::proc::StepPtr step)
{
  this->mStep = step;
  this->mClassId = cedar::proc::Manager::getInstance().steps().getDeclarationOf(this->mStep);

  this->mStepIcon = QIcon(this->mClassId->getIconPath().c_str());
  if (this->mStepIcon.isNull())
  {
    this->mStepIcon = QIcon(":/steps/no_icon.svg");
  }

  this->addDataItems();
  this->addTriggerItems();

  QObject::connect(step.get(), SIGNAL(stateChanged()), this, SLOT(stepStateChanged()));
}

void cedar::proc::gui::StepItem::readConfiguration(const cedar::aux::ConfigurationNode& node)
{
  this->cedar::proc::gui::GraphicsBase::readConfiguration(node);
  try
  {
    std::string step_name = node.get<std::string>("step");
    cedar::proc::StepPtr step = cedar::proc::Manager::getInstance().steps().get(step_name);
    this->setStep(step);
  }
  catch (const boost::property_tree::ptree_bad_path&)
  {
    CEDAR_THROW(cedar::proc::gui::InvalidStepNameException, "Cannot read StepItem from file: no step name given.");
  }
}

void cedar::proc::gui::StepItem::saveConfiguration(cedar::aux::ConfigurationNode& root)
{
  root.put("step", this->mStep->getName());
  this->cedar::proc::gui::GraphicsBase::saveConfiguration(root);
}

void cedar::proc::gui::StepItem::addTriggerItems()
{
  CEDAR_DEBUG_ASSERT(this->mStep);

  qreal padding = static_cast<qreal>(3);
  QPointF origin(0, this->height() + padding);
  qreal trigger_size = 10.0;
  mTriggers.clear();

  for (size_t i = 0; i < this->mStep->getTriggerCount(); ++i)
  {
    cedar::proc::TriggerPtr trigger = this->mStep->getTrigger(i);
    cedar::proc::gui::TriggerItem* p_trigger_item = new cedar::proc::gui::TriggerItem(trigger);
    p_trigger_item->setParentItem(this);
    p_trigger_item->setPos(origin + QPointF(0, 1) * static_cast<qreal>(i) * (trigger_size + padding));
    p_trigger_item->isDocked(true);
    p_trigger_item->setWidth(trigger_size);
    p_trigger_item->setHeight(trigger_size);
    this->mTriggers.push_back(p_trigger_item);
  }
}

void cedar::proc::gui::StepItem::addDataItems()
{
  qreal data_size = 10.0; //!@todo don't hard-code the size of the data items
  qreal padding = static_cast<qreal>(3);
  std::map<cedar::proc::DataRole::Id, QPointF> add_origins;
  std::map<cedar::proc::DataRole::Id, QPointF> add_directions;

  add_origins[cedar::proc::DataRole::BUFFER] = QPointF(0, -padding - data_size);
  add_directions[cedar::proc::DataRole::BUFFER] = QPointF(1, 0);

  add_origins[cedar::proc::DataRole::INPUT] = QPointF(-padding - data_size, 0);
  add_directions[cedar::proc::DataRole::INPUT] = QPointF(0, 1);

  add_origins[cedar::proc::DataRole::OUTPUT] = QPointF(this->width() + padding, 0);
  add_directions[cedar::proc::DataRole::OUTPUT] = QPointF(0, 1);

  for (std::vector<cedar::aux::Enum>::const_iterator enum_it = cedar::proc::DataRole::type().list().begin();
      enum_it != cedar::proc::DataRole::type().list().end();
      ++enum_it)
  {
    if ( (*enum_it) == cedar::aux::Enum::UNDEFINED)
      continue;

    // populate step item list
    mSlotMap[*enum_it] = DataSlotNameMap();

#ifdef DEBUG
    if (add_origins.find(*enum_it) == add_origins.end() || add_directions.find(*enum_it) == add_directions.end())
    {
      std::cout << "Warning: the data role " << enum_it->prettyString() << " is not implemented properly in "
                   "StepItem::addDataItems. Skipping!" << std::endl;
      continue;
    }
#endif

    const QPointF& origin = add_origins[*enum_it];
    const QPointF& direction = add_directions[*enum_it];

    try
    {
      qreal count = 0;
      cedar::proc::Step::SlotMap& slotmap = this->mStep->getDataSlots(*enum_it);
      for (cedar::proc::Step::SlotMap::iterator iter = slotmap.begin(); iter != slotmap.end(); ++iter)
      {
        cedar::proc::DataSlotPtr slot = iter->second;
        cedar::proc::gui::DataSlotItem *p_item = new cedar::proc::gui::DataSlotItem(this, slot);
        p_item->setPos(origin + count * direction * (data_size + padding) );
        mSlotMap[slot->getRole()][slot->getName()] = p_item;
        count += static_cast<qreal>(1.0);
      }
    }
    catch(const cedar::proc::InvalidRoleException&)
    {
      // ok -- a step may not have any data for this role.
    }
  }
}

cedar::proc::gui::DataSlotItem* cedar::proc::gui::StepItem::getSlotItem
                                                              (
                                                                cedar::proc::DataRole::Id role, const std::string& name
                                                              )
{
  DataSlotMap::iterator role_map = this->mSlotMap.find(role);

  if (role_map == this->mSlotMap.end())
  {
    CEDAR_THROW(cedar::proc::InvalidRoleException, "No slot items stored for role "
                                                   + cedar::proc::DataRole::type().get(role).prettyString()
                                                   );
  }

  DataSlotNameMap::iterator iter = role_map->second.find(name);
  if (iter == role_map->second.end())
  {
    CEDAR_THROW(cedar::proc::InvalidNameException, "No slot item named \"" + name +
                                                   "\" found for role "
                                                   + cedar::proc::DataRole::type().get(role).prettyString()
                                                   + " in StepItem for step \"" + this->mStep->getName() + "\"."
                                                   );
  }

  return iter->second;
}

cedar::proc::gui::StepItem::DataSlotNameMap& cedar::proc::gui::StepItem::getSlotItems(
                                                                             cedar::proc::DataRole::Id role
                                                                           )
{
  DataSlotMap::iterator role_map = this->mSlotMap.find(role);

  if (role_map == this->mSlotMap.end())
  {
    CEDAR_THROW(cedar::proc::InvalidRoleException, "Unknown role  "
                                                   + cedar::proc::DataRole::type().get(role).prettyString()
                                                   );
  }
  return role_map->second;
}

void cedar::proc::gui::StepItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
  QMenu menu;
  QMenu* p_data = menu.addMenu("data");
  menu.addSeparator();
  QAction *p_delete_action = menu.addAction("delete");

  std::map<QAction*, cedar::aux::Enum> action_type_map;
  bool has_data = false;

  for (std::vector<cedar::aux::Enum>::const_iterator enum_it = cedar::proc::DataRole::type().list().begin();
      enum_it != cedar::proc::DataRole::type().list().end();
      ++enum_it)
  {
    try
    {
      const cedar::aux::Enum& e = *enum_it;
      const cedar::proc::Step::SlotMap& slotmap = this->mStep->getDataSlots(e.id());
      std::string label = e.prettyString() + "s";
      p_data->addSeparator();
      QAction *p_label_action = p_data->addAction(label.c_str());
      QFont font = p_label_action->font();
      font.setBold(true);
      p_label_action->setFont(font);
      p_data->addSeparator();
      p_label_action->setEnabled(false);
      for (cedar::proc::Step::SlotMap::const_iterator iter = slotmap.begin(); iter != slotmap.end(); ++iter)
      {
        QAction *p_action = p_data->addAction(iter->second->getText().c_str());
        p_action->setData(QString(iter->first.c_str()));
        if (iter->second->getData().get() == NULL)
        {
          p_action->setText("[unconnected] " + p_action->text());
          p_action->setEnabled(false);
        }
        action_type_map[p_action] = e;
        has_data = true;
      }
    }
    catch (const cedar::proc::InvalidRoleException& e)
    {
      // that's ok, a step may not have any data in a certain role.
    }
  }

  if (!has_data)
  {
    QAction *p_action = p_data->addAction("no data");
    p_action->setEnabled(false);
  }

  QAction *a = menu.exec(event->screenPos());

  if (a == NULL)
    return;

  if (a->parentWidget() == p_data)
  {
    std::string data_name = a->data().toString().toStdString();
    const cedar::aux::Enum& e = action_type_map[a];
    cedar::aux::DataPtr p_data = this->mStep->getData(e, data_name);

    std::string title = this->mStep->getSlot(e, data_name)->getText();
    title += " (" + this->mStep->getName();
    title += "." + data_name + ")";

    cedar::proc::gui::DataPlotter *p_plotter = new cedar::proc::gui::DataPlotter(title, mpMainWindow);
    p_plotter->plot(p_data);
    p_plotter->show();
  }
  else if (a == p_delete_action)
  {
    //!@todo
  }
}

void cedar::proc::gui::StepItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* style, QWidget* widget)
{
  painter->save(); // save current painter settings

  //! @todo make drawing of steps pretty.

  qreal padding = 5;
  qreal icon_dim = 40;

  this->paintFrame(painter, style, widget);
  this->mStepIcon.paint(painter, padding, padding, icon_dim, icon_dim);
  painter->drawText(QPointF(2 * padding + icon_dim, 15), this->mClassId->getClassName().c_str());
  painter->drawText(QPointF(2 * padding + icon_dim, 25), this->mStep->getName().c_str());

  painter->restore(); // restore saved painter settings
}

cedar::proc::StepPtr cedar::proc::gui::StepItem::getStep()
{
  return this->mStep;
}

void cedar::proc::gui::StepItem::disconnect()
{
  // go through all DataSlots and remove connections
  for (size_t i = 0; i < cedar::proc::DataRole::type().list().size(); ++i)
  {
    cedar::proc::DataRole::Id id = cedar::proc::DataRole::type().list().at(i);
    if (id == cedar::aux::Enum::UNDEFINED)
    {
      continue;
    }
    cedar::proc::gui::StepItem::DataSlotNameMap& map = dynamic_cast<cedar::proc::gui::StepItem*>(this)->getSlotItems(id);
    for (cedar::proc::gui::StepItem::DataSlotNameMap::iterator it = map.begin(); it != map.end(); ++it)
    {
      it->second->removeAllConnections();
    }
  }

  // go through all triggers and remove their connections as well
  for (size_t i = 0; i < this->mTriggers.size(); ++i)
  {
    cedar::proc::gui::TriggerItem* p_trigger = this->mTriggers.at(i);
    p_trigger->removeAllConnections();
  }
}
