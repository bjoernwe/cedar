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

    File:        StickyNote.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.rub.de
    Date:        2013 07 01

    Description: Header of the @em cedar::proc::gui::StickyNote class.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_STICKY_NOTE_H
#define CEDAR_PROC_STICKY_NOTE_H

// CEDAR INCLUDES
#include "cedar/processing/gui/Scene.h"

// FORWARD DECLARATIONS
#include "cedar/processing/gui/StickyNote.fwd.h"

// SYSTEM INCLUDES
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QRectF>
#include <QPainter>
#include <QWidget>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneEvent>

//!@brief A GUI class that creates a pane where the user can write a comment or note.
class cedar::proc::gui::StickyNote: public QGraphicsItem
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------

public:
  //!@brief the constructor
  StickyNote(cedar::proc::gui::Scene* pParent, int x, int y, int width = 120, int height = 70, std::string text = "");

  //!@brief the destructor
  ~StickyNote();


  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:

  //!@brief Returns the most outer rect of this element.
  QRectF boundingRect() const;

  //!@brief Paints the sticky note.
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  //!@brief Checks if an key was pressed.
  void keyPressEvent(QKeyEvent* event);

  //!@brief Event when mouse is pressed on the note.
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

  //!@brief Event when mouse is released from the note.
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

  //!@brief Events when mouse is moved in the note.
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

  void dragMoveEvent(QGraphicsSceneDragDropEvent *event);

  std::string getText() const;

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
private:

  //!@brief The most outer rect.
  QRectF mBound;

  //!@brief The text inside the note.
  QTextEdit mText;

  //!@brief A helper for storing QWidgets in a QGraphicsItem .
  QGraphicsProxyWidget *mpProxy;

  //!@brief The color of the note.
  QColor mColor;

  //!@brief A reference to the parent scene.
  cedar::proc::gui::Scene* mpParent;

  //!@brief flag that saves if the not is currently in scaling mode
  bool mScaling;
};

#endif // CEDAR_PROC_STICKY_NOTE_H
