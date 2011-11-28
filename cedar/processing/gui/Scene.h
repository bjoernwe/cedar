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

    File:        Scene.h

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 07 05

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_SCENE_H
#define CEDAR_PROC_SCENE_H

// LOCAL INCLUDES
#include "cedar/processing/gui/namespace.h"
#include "cedar/processing/namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <QGraphicsScene>
#include <QMainWindow>


/*!@brief Abstract description of the class.
 *
 * More detailed description of the class.
 */
class cedar::proc::gui::Scene : public QGraphicsScene
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros & types
  //--------------------------------------------------------------------------------------------------------------------
  Q_OBJECT

public:
  enum MODE
  {
    MODE_SELECT,
    MODE_CONNECT
  };

  typedef std::map<cedar::proc::Step*, cedar::proc::gui::StepItem*> StepMap;
  typedef std::map<cedar::proc::Trigger*, cedar::proc::gui::TriggerItem*> TriggerMap;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  Scene(cedar::proc::gui::View* peParentView, QObject *pParent = NULL, QMainWindow *pMainWindow = NULL);

  //!@brief Destructor
  ~Scene();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  void dropEvent(QGraphicsSceneDragDropEvent *pEvent);
  void dragEnterEvent(QGraphicsSceneDragDropEvent *pEvent);
  void dragMoveEvent(QGraphicsSceneDragDropEvent *pEvent);

  void mousePressEvent(QGraphicsSceneMouseEvent *pMouseEvent);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *pMouseEvent);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *pMouseEvent);

  void addElement(const std::string& classId, QPointF position);
  void addProcessingStep(cedar::proc::StepPtr step, QPointF position);
  void addStepItem(cedar::proc::gui::StepItem *pStep);
  void removeStepItem(cedar::proc::gui::StepItem *pStep);
  void addTrigger(cedar::proc::TriggerPtr trigger, QPointF position);
  void addTriggerItem(cedar::proc::gui::TriggerItem *pTrigger);
  void removeTriggerItem(cedar::proc::gui::TriggerItem *pTrigger);

  void setMode(MODE mode, const QString& param = "");

  void setMainWindow(QMainWindow *pMainWindow);

  void setNetwork(cedar::proc::gui::NetworkFilePtr network);

  void reset();

  const StepMap& stepMap() const;
  const TriggerMap& triggerMap() const;

  cedar::proc::gui::StepItem* getStepItemFor(cedar::proc::Step* step);
  cedar::proc::gui::TriggerItem* getTriggerItemFor(cedar::proc::Trigger* trigger);

  bool getSnapToGrid() const;
  void setSnapToGrid(bool snap);

  //--------------------------------------------------------------------------------------------------------------------
  // signals
  //--------------------------------------------------------------------------------------------------------------------
  signals:
    void exception(const QString& message);
    void modeFinished();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  /* connect mode */
  void connectModeProcessMousePress(QGraphicsSceneMouseEvent *pMouseEvent);
  void connectModeProcessMouseMove(QGraphicsSceneMouseEvent *pMouseEvent);
  void connectModeProcessMouseRelease(QGraphicsSceneMouseEvent *pMouseEvent);

  /* group mode */
  void groupModeProcessMousePress(QGraphicsSceneMouseEvent *pMouseEvent);
  void groupModeProcessMouseMove(QGraphicsSceneMouseEvent *pMouseEvent);
  void groupModeProcessMouseRelease(QGraphicsSceneMouseEvent *pMouseEvent);

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet
private:
  MODE mMode;
  QString mModeParam;

  cedar::proc::gui::NetworkFilePtr mNetwork;
  cedar::proc::gui::View *mpeParentView;
  /* connect mode related */
  QGraphicsLineItem *mpNewConnectionIndicator;
  cedar::proc::gui::GraphicsBase *mpConnectionStart;

  /* group mode related */
  QPointF mGroupStart;
  QPointF mGroupEnd;
  QGraphicsRectItem *mpGroupIndicator;
  QList<QGraphicsItem*> mProspectiveGroupMembers;
  QMainWindow *mpMainWindow;

  StepMap mStepMap;
  TriggerMap mTriggerMap;

  bool mSnapToGrid;

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
public:
  // none yet (hopefully never!)
protected:
  // none yet

private:
  // none yet

}; // class ProcessingScene

#endif // CEDAR_PROC_SCENE_H

