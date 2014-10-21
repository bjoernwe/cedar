/*======================================================================================================================

    Copyright 2011, 2012, 2013, 2014 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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
#include "cedar/processing/gui/ui_Ide.h"
#include "cedar/auxiliaries/LogInterface.h"
#include "cedar/auxiliaries/LockableMember.h"

// FORWARD DECLARATIONS
#include "cedar/auxiliaries/CallFunctionInThread.fwd.h"
#include "cedar/processing/gui/PerformanceOverview.fwd.h"
#include "cedar/processing/gui/ArchitectureConsistencyCheck.fwd.h"
#include "cedar/processing/gui/BoostControl.fwd.h"
#include "cedar/processing/gui/ExperimentDialog.fwd.h"
#include "cedar/processing/gui/ElementClassList.fwd.h"
#include "cedar/processing/gui/Ide.fwd.h"
#include "cedar/processing/gui/Group.fwd.h"
#include "cedar/processing/gui/FindDialog.fwd.h"

// SYSTEM INCLUDES
#include <QMainWindow>
#include <QKeyEvent>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <map>


/*!@brief This is the main window of the cedar application.
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
  /*!@brief The standard constructor.
   *
   * @param loadDefaultPlugins Loads all plugins set as default in the configuration.
   * @param redirectLogToGui   Enables or disables redirection of log messages to the gui (can help when too many log
   *                           messages lock up the user interface).
   */
  Ide(bool loadDefaultPlugins = true, bool redirectLogToGui = true);

  //!@brief Destructor
  ~Ide();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  /*!@brief Handles key presses in the main window.
   */
  void keyPressEvent(QKeyEvent* pEvent);

  /*!@brief Resets the current scene and displays the new network.
   */
  void resetTo(cedar::proc::gui::GroupPtr network);

  cedar::proc::gui::GroupPtr getGroup();
  
  //! set if close dialog should be suppressed
  void suppressCloseDialog(bool suppress)
  {
    this->mSuppressCloseDialog = suppress;
  }

public slots:
  /*!@brief Slot that displays notifications.
   */
  void notify(const QString& message);

  /*!@brief Changes the mode back to select when an architecture tool is finished.
   */
  void architectureToolFinished();
  
  /*!@brief Slot that is connected to the start/pause simulation action.
   */
  void startPauseSimulationClicked();

  /*!@brief Stops all looped triggers (and other derivatives of looped thread).
   */
  void resetSimulationClicked();

  /*!@brief Single-step all looped triggers (and other derivatives of looped thread).
   */
  void stepThreads();

  /*!@brief Slot that is connected to the "new" item in the file menu.
   */
  void newFile();

  /*!@brief Slot that is connected to the "save" item in the file menu.
   */
  bool save();

  /*!@brief Slot that is connected to the "save as" item in the file menu.
   */
  bool saveAs();

  //! Allows the user to choose a destination for saving the data of serializable data slots in all steps.
  bool saveSerializableDataAs();

  //! Allows the user to choose a source for loading data into serializable data slots of all steps.
  bool loadSerializableData();

  /*!@brief Slot that is connected to the "load" item in the file menu.
   */
  void load();

  /*!@brief Loads a network file specified by the path.
   */
  void loadFile(QString path);

  /*!@brief Opens the manage plugins dialog.
   */
  void showManagePluginsDialog();

  /*!@brief Shows the settings dialog.
   */
  void showSettingsDialog();

  /*!@brief Toggles the snap to grid functionality.
   */
  void toggleGrid(bool toggled);

  /*!@brief Fills the recent files list.
   */
  void fillRecentFilesList();

  /*!@brief Slot that is connected to clicking the entries in the recent files list.
   */
  void recentFileItemTriggered();

  /*!@brief Returns the architecture view used by the ide.
   */
  cedar::proc::gui::View* getArchitectureView();

  /*!@brief Displays the about dialog.
   */
  void showAboutDialog();

  /*!@brief Resets the root network
   */
  void resetRootGroup();

  /*!@brief brakes all running robots
   */
  void brakeAllRobots();


  /*!@brief Opens a dialog that lets the user export the current scene as an SVG
   */
  void exportSvg();

  /*!@brief Opens a dialog that contains the robot manager widget.
   */
  void showRobotManager();
  
  /*!@brief Duplicates a selected step
   */
  void duplicateStep();

  /*!@brief Select all elements
   */
  void selectAll();

  /*!@brief Show/hide all trigger connections
   */
  void showTriggerConnections(bool show);

  /*!@brief Shows a dialog for architecture consistency checking.
   */
  void showConsistencyChecker();

  /*!@brief Opens a boost control widget.
   */
  void showBoostControl();

  /*! Opens the parameter linker
   */
  void openParameterLinker();

  //!@brief toggle smart connections
  void toggleSmartConnections(bool smart);

  //!@brief closes all plot windows of every step
  void closePlots();

  //!@brief shows/hides all plot windows of every step
  void toggleVisibilityOfPlots(bool hidden = false);

  //!@brief shows the experiment dialog widget
  void showExperimentDialog();

  //!@brief Starts or stops the recorder function();
  void toggleRecorder(bool status);

  //!@brief Takes a snap shot from the registered steps;
  void takeSnapshot();

  //!@brief opens dialogue to add plotgroup
  void addPlotGroup();
  
  //!@brief opens dialogue to edit selected plotgroup
  void editPlotGroup();

  //!@brief displays the selected plotgroup
  void displayPlotGroup();

  //!@brief deletes the selected plotgroup
  void deletePlotGroup();
  //! Returns the log widget of this ide.
  cedar::aux::gui::Log* getLog() const
  {
    return this->mpLog;
  }

  //!@brief copy one step to buffer
  void copyStep();

  //!@brief copy configuration to target step(s)
  void pasteStepConfiguration();

  //!@brief copy one step to buffer
  void openFindDialog();

  //! return the gui root group
  cedar::proc::gui::ConstGroupPtr getGroup() const;

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:

  /*!@brief Deletes the list of graphics items.
   */
  void deleteElements(QList<QGraphicsItem*>& items);

  /*!@brief Delete a single graphics item.
   */
  void deleteElement(QGraphicsItem* pItem);

  /*!@brief Deletes the elements currently selected in the scene.
   */
  void deleteSelectedElements();

  /*!@brief Reacts to closing the window.
   */
  void closeEvent(QCloseEvent *pEvent);

  //! Periodically updates certain information
  void timerEvent(QTimerEvent*);

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

  /*!@brief Sets the filename in the title of the main window.
   */
  void displayFilename(const std::string& filename);

  /*!@brief sort two QGraphicsItems measuring their depth in relation to the root network.
   */
  static bool sortElements(QGraphicsItem* pFirstItem, QGraphicsItem* pSecondItem);

  //!@brief populates the Plot Groups Combobox with available Plot Groups
  void loadPlotGroupsIntoComboBox();

  void setGroup(cedar::proc::gui::GroupPtr group);

  void setArchitectureChanged(bool changed);

  //! Check if the user wants to save. Returns false if the action currently being taken should be cancelled.
  bool checkSave();

  //! Updates the architecture widget menu.
  void updateArchitectureWidgetsMenu();

  //! Constructs the widgets in the status bar.
  void buildStatusBar();

  void updateSimulationRunningIcon(bool running);

private slots:
  void globalTimeFactorSliderChanged(int newValue);

  void globalTimeFactorSpinboxChanged(double value);

  void architectureChanged();

  void architecturePlotActionTriggered();

  void openManageArchitectureWidgetsDialog();

  void setSimulationControlsDisabled(bool disabled);

  void triggerStarted();

  void allTriggersStopped();

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  //! Widgets for each list of element classes.
  std::map<std::string, cedar::proc::gui::ElementClassList*> mElementClassListWidgets;

  //! The network currently displayed.
  cedar::proc::gui::GroupPtr mGroup;

  cedar::proc::StepPtr mLastCopiedStep;

  //! Architecture consistency check widget.
  cedar::proc::gui::ArchitectureConsistencyCheck* mpConsistencyChecker;

  //! Performance overview.
  cedar::proc::gui::PerformanceOverview* mpPerformanceOverview;

  //! Dock widget for the consistency checker.
  QDockWidget* mpConsistencyDock;

  //! Dock widget for the boost control
  QDockWidget* mpBoostControlDock;

  QString mDefaultWindowTitle;

  cedar::proc::gui::BoostControl* mpBoostControl;

  //! In which the user specifies the time step for single-step functionality.
  QDoubleSpinBox* mpCustomTimeStep;

  //! Combobox to select plot groups
  QComboBox* mpPlotGroupsComboBox;

  //! Spinbox for controlling the global time step.
  QDoubleSpinBox* mpGlobalTimeFactor;

  //! Spinbox for controlling the global time step.
  QSlider* mpGlobalTimeFactorSlider;

  //! Whether the save on close dialog should be suppressed.
  bool mSuppressCloseDialog;

  cedar::proc::gui::FindDialog* mpFindDialog;

  //! Widget for creating and running experiments
  cedar::proc::gui::ExperimentDialog* mpExperimentDialog;

  //! Label used for displaying the current global time.
  QLabel* mpGlobalTimeLabel;

  cedar::aux::LockableMember<bool> mSimulationRunning;

}; // class cedar::MainWindow

#endif // CEDAR_PROC_GUI_MAIN_WINDOW_H
