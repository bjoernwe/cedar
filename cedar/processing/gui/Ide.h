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

    File:        Ide.h

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

#ifndef CEDAR_PROC_GUI_MAIN_WINDOW_H
#define CEDAR_PROC_GUI_MAIN_WINDOW_H

// CEDAR INCLUDES
#ifdef DEBUG
  #include "cedar/processing/debug/gui/ui_Ide.h"
#else
  #include "cedar/processing/release/gui/ui_Ide.h"
#endif
#include "cedar/processing/gui/namespace.h"

// SYSTEM INCLUDES
#include <QMainWindow>
#include <QKeyEvent>
#include <map>


/*!@brief This is the main window of the processingIde application.
 */
class cedar::proc::gui::Ide : public QMainWindow, public Ui_Ide
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------
  Q_OBJECT

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  Ide();

  //!@brief Destructor
  ~Ide();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Handles key presses in the main window.
   */
  void keyPressEvent(QKeyEvent* pEvent);

public slots:
  /*!@brief Slot that is called whenever a different item is selected in the cedar::proc::gui::Scene.
   *
   * @todo This probably belongs somewhere else.
   */
  void sceneItemSelected();

  /*!@brief Slot that displays exceptions.
   */
  void exception(const QString& message);

  /*!@brief Slot that displays notifications.
   */
  void notify(const QString& message);

  /*!@brief Slot that displays errors.
   */
  void error(const QString& message);

  /*!@brief Slot that displays messages.
   */
  void message(const QString& message);

  /*!@brief Changes the mode back to select when an architecture tool is finished.
   */
  void architectureToolFinished();
  
  /*!@brief Starts all looped triggers (and other derivatives of looped thread).
   */
  void startThreads();

  /*!@brief Stops all looped triggers (and other derivatives of looped thread).
   */
  void stopThreads();

  /*!@brief Slot that is connected to the "new" item in the file menu.
   */
  void newFile();

  /*!@brief Slot that is connected to the "save" item in the file menu.
   */
  void save();

  /*!@brief Slot that is connected to the "save as" item in the file menu.
   */
  void saveAs();

  /*!@brief Slot that is connected to the "load" item in the file menu.
   */
  void load();

  /*!@brief Loads a network file specified by the path.
   */
  void loadFile(QString path);

  /*!@brief Resets the current scene and displays the new network.
   */
  void resetTo(cedar::proc::gui::NetworkFilePtr network);

  /*!@brief Opens the load plugin dialog.
   */
  void showLoadPluginDialog();

  /*!@brief Opens the manage plugins dialog.
   */
  void showManagePluginsDialog();

  /*!@brief Toggles the snap to grid functionality.
   */
  void toggleGrid(bool toggled);

  /*!@brief Fills the recent files list.
   */
  void fillRecentFilesList();

  /*!@brief Slot that is connected to clicking the entries in the recent files list.
   */
  void recentFileItemTriggered();

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  /*!@brief Deletes the list of gragics items.
   *
   * @todo This probably belongs somewhere else, e.g., cedar::proc::gui::Scene.
   */
  void deleteElements(QList<QGraphicsItem*>& items);

  /*!@brief Deletes the elements currently selected in the scene.
   *
   * @todo This probably belongs somewhere else, e.g., cedar::proc::gui::Scene.
   */
  void deleteSelectedElements();

  /*!@brief Reacts to closing the window.
   */
  void closeEvent(QCloseEvent *pEvent);

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  /*!@brief Resets the list of elements.
   */
  void resetStepList();

  /*!@brief Loads the default plugins.
   */
  void loadDefaultPlugins();

  /*!@brief Stores the window state and other settings.
   */
  void storeSettings();

  /*!@brief Restores window state and other settings that were previously stored with
   *        cedar::proc::gui::Ide::storeSettings.
   */
  void restoreSettings();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //! Widgets for each list of element classes.
  std::map<std::string, cedar::proc::gui::ElementClassList*> mElementClassListWidgets;

  //! The network currently displayed.
  cedar::proc::gui::NetworkFilePtr mNetwork;

}; // class cedar::MainWindow

#endif // CEDAR_PROC_GUI_MAIN_WINDOW_H

