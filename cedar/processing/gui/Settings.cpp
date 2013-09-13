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

    File:        FrameworkSettings.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 26

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/processing/gui/Settings.h"
#include "cedar/auxiliaries/PluginProxy.h"
#include "cedar/auxiliaries/Configurable.h"
#include "cedar/auxiliaries/SetParameter.h"
#include "cedar/auxiliaries/DirectoryParameter.h"
#include "cedar/auxiliaries/VectorParameter.h"
#include "cedar/auxiliaries/StringParameter.h"
#include "cedar/auxiliaries/systemFunctions.h"

// SYSTEM INCLUDES
#include <boost/property_tree/json_parser.hpp>
#include <QMainWindow>

cedar::aux::EnumType<cedar::proc::gui::Settings::StepDisplayMode>
  cedar::proc::gui::Settings::StepDisplayMode::mType("cedar::proc::gui::Settings::StepDisplayMode::");

#ifndef CEDAR_COMPILER_MSVC
const cedar::proc::gui::Settings::StepDisplayMode::Id cedar::proc::gui::Settings::StepDisplayMode::ICON_ONLY;
const cedar::proc::gui::Settings::StepDisplayMode::Id cedar::proc::gui::Settings::StepDisplayMode::TEXT_FOR_LOOPED;
const cedar::proc::gui::Settings::StepDisplayMode::Id cedar::proc::gui::Settings::StepDisplayMode::ICON_AND_TEXT;
#endif // CEDAR_COMPILER_MSVC

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::gui::Settings::Settings()
:
cedar::aux::Configurable(),
mWritingDisabled(false),
mLog(new cedar::proc::gui::Settings::DockSettings()),
mSteps(new cedar::proc::gui::Settings::DockSettings()),
mTools(new cedar::proc::gui::Settings::DockSettings()),
mProperties(new cedar::proc::gui::Settings::DockSettings()),
mMainWindowGeometry(new cedar::aux::StringParameter(this, "mainWindowGeometry", "")),
mMainWindowState(new cedar::aux::StringParameter(this, "mainWindowState", ""))
{
  cedar::aux::ConfigurablePtr ui_settings(new cedar::aux::Configurable());
  this->addConfigurableChild("ui", ui_settings);
  
  ui_settings->addConfigurableChild("log", mLog);
  ui_settings->addConfigurableChild("steps", mSteps);
  ui_settings->addConfigurableChild("tools", mTools);
  ui_settings->addConfigurableChild("properties", mProperties);


  cedar::aux::ConfigurablePtr slot_growth(new cedar::aux::Configurable());
  this->addConfigurableChild("slot growth", slot_growth);

  this->_mDataSlotScalingEnabled = new cedar::aux::BoolParameter(slot_growth.get(), "enabled", true);
  auto growth_limits = cedar::aux::DoubleParameter::LimitType::positive();
  growth_limits.setLower(1.0);
  this->_mDataSlotScaling = new cedar::aux::DoubleParameter(slot_growth.get(), "factor", 1.3, growth_limits);

  this->_mDataSlotScalingSensitivity
    = new cedar::aux::DoubleParameter(slot_growth.get(), "sensitivity", 10.0, growth_limits);


  this->mSnapToGrid = cedar::aux::BoolParameterPtr
                      (
                        new cedar::aux::BoolParameter
                        (
                          ui_settings.get(),
                          "snapToGrid",
                          false
                        )
                      );

  cedar::aux::ConfigurablePtr display_settings(new cedar::aux::Configurable());
  this->addConfigurableChild("displaySettings", display_settings);
  mUseGraphicsItemShadowEffects = cedar::aux::BoolParameterPtr
                                  (
                                    new cedar::aux::BoolParameter
                                    (
                                      display_settings.get(),
                                      "useGraphicsItemShadowEffects",
                                      false
                                    )
                                  );

  this->_mDefaultStepDisplayMode = cedar::aux::EnumParameterPtr
      (
        new cedar::aux::EnumParameter
        (
          display_settings.get(),
          "default step display mode",
          cedar::proc::gui::Settings::StepDisplayMode::typePtr(),
          cedar::proc::gui::Settings::StepDisplayMode::TEXT_FOR_LOOPED
        )
      );

  cedar::aux::ConfigurablePtr recent_files(new cedar::aux::Configurable());
  this->addConfigurableChild("fileHistory", recent_files);
  this->_mMaxFileHistorySize = new cedar::aux::UIntParameter(recent_files.get(), "maximum history size", 10);

  this->mPluginLoadDialogLocation = cedar::aux::DirectoryParameterPtr
      (
        new cedar::aux::DirectoryParameter
        (
          recent_files.get(),
          "lastPluginLoadDialogLocation",
          ""
        )
      );
  this->mArchitectureLoadDialogDirectory = cedar::aux::DirectoryParameterPtr
      (
        new cedar::aux::DirectoryParameter
        (
          recent_files.get(),
          "lastArchitectureLoadDialogDirectory",
          ""
        )
      );
  this->mArchitectureExportDialogDirectory = cedar::aux::DirectoryParameterPtr
      (
        new cedar::aux::DirectoryParameter
        (
          recent_files.get(),
          "lastArchitectureExportDialogDirectory",
          ""
        )
      );
  this->mRecentArchitectureFiles = cedar::aux::StringVectorParameterPtr
      (
        new cedar::aux::StringVectorParameter
        (
          recent_files.get(),
          "architectureFileHistory",
          std::vector<std::string>()
        )
      );

  this->load();
}

cedar::proc::gui::Settings::DockSettings::DockSettings()
:
mVisible(new cedar::aux::BoolParameter(this, "visible", true)),
mFloating(new cedar::aux::BoolParameter(this, "floating", false))
{
}

cedar::proc::gui::Settings::~Settings()
{
  this->save();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

bool cedar::proc::gui::Settings::snapToGrid() const
{
  return this->mSnapToGrid->getValue();
}

void cedar::proc::gui::Settings::snapToGrid(bool snap)
{
  this->mSnapToGrid->setValue(snap);
}

bool cedar::proc::gui::Settings::useGraphicsItemShadowEffects() const
{
  return this->mUseGraphicsItemShadowEffects->getValue();
}

void cedar::proc::gui::Settings::setUseGraphicsItemShadowEffects(bool useShadows)
{
  this->mUseGraphicsItemShadowEffects->setValue(useShadows);
}

void cedar::proc::gui::Settings::appendArchitectureFileToHistory(const std::string& filePath)
{
  std::vector<std::string> new_order;
  if (!this->mRecentArchitectureFiles->contains(filePath))
  {
    new_order = this->mRecentArchitectureFiles->getValue();
    new_order.push_back(filePath);
  }
  else
  {
    for (size_t i = 0; i < this->mRecentArchitectureFiles->size(); ++i)
    {
      const std::string& value = this->mRecentArchitectureFiles->at(i);
      if (value != filePath)
      {
        new_order.push_back(value);
      }
    }

    new_order.push_back(filePath);
  }

  while (new_order.size() > this->_mMaxFileHistorySize->getValue())
  {
    new_order.erase(new_order.begin());
  }
  this->mRecentArchitectureFiles->set(new_order);
}

cedar::aux::StringVectorParameterPtr cedar::proc::gui::Settings::getArchitectureFileHistory()
{
  return this->mRecentArchitectureFiles;
}

cedar::aux::DirectoryParameterPtr cedar::proc::gui::Settings::lastArchitectureLoadDialogDirectory()
{
  return this->mArchitectureLoadDialogDirectory;
}

cedar::aux::DirectoryParameterPtr cedar::proc::gui::Settings::lastArchitectureExportDialogDirectory()
{
  return this->mArchitectureExportDialogDirectory;
}

cedar::aux::DirectoryParameterPtr cedar::proc::gui::Settings::lastPluginLoadDialogLocation()
{
  return this->mPluginLoadDialogLocation;
}

void cedar::proc::gui::Settings::storeMainWindow(QMainWindow *pWindow)
{
  QByteArray window_state = pWindow->saveState();
  QByteArray window_state_hex = window_state.toHex();
  mMainWindowState->setValue(window_state_hex.constData());
  
  QByteArray window_geometry = pWindow->saveGeometry();
  QByteArray window_geometry_hex = window_geometry.toHex();
  mMainWindowGeometry->setValue(window_geometry_hex.constData());
}

void cedar::proc::gui::Settings::restoreMainWindow(QMainWindow *pWindow)
{
  QByteArray window_state_hex(mMainWindowState->getValue().c_str());
  QByteArray window_state = QByteArray::fromHex(window_state_hex);
  if (!pWindow->restoreState(window_state))
  {
    std::cout << "Could not restore state of the main window." << std::endl;
  }
  
  QByteArray window_geometry_hex(mMainWindowGeometry->getValue().c_str());
  QByteArray window_geometry = QByteArray::fromHex(window_geometry_hex);
  if (!pWindow->restoreGeometry(window_geometry))
  {
    std::cout << "Could not restore geometry of the main window." << std::endl;
  }
}

void cedar::proc::gui::Settings::DockSettings::getFrom(QDockWidget *pDock)
{
  this->mVisible->setValue(pDock->isVisible());
  this->mFloating->setValue(pDock->isFloating());
}

void cedar::proc::gui::Settings::DockSettings::setTo(QDockWidget *pDock)
{
  pDock->setVisible(this->mVisible->getValue());
  pDock->setFloating(this->mFloating->getValue());
}

cedar::proc::gui::Settings::DockSettingsPtr cedar::proc::gui::Settings::logSettings()
{
  return this->mLog;
}

cedar::proc::gui::Settings::DockSettingsPtr cedar::proc::gui::Settings::toolsSettings()
{
  return this->mTools;
}

cedar::proc::gui::Settings::DockSettingsPtr cedar::proc::gui::Settings::propertiesSettings()
{
  return this->mProperties;
}

cedar::proc::gui::Settings::DockSettingsPtr cedar::proc::gui::Settings::stepsSettings()
{
  return this->mSteps;
}

void cedar::proc::gui::Settings::load()
{
  std::string path = cedar::aux::getUserApplicationDataDirectory() + "/.cedar/processingGui";
  try
  {
    this->readJson(path);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    cedar::aux::LogSingleton::getInstance()->warning
    (
      std::string("Error reading framework gui settings: ") + e.what(),
      "void cedar::proc::gui::Settings::load()"
    );
  }
}

void cedar::proc::gui::Settings::save()
{
  if (!mWritingDisabled)
  {
    std::string path = cedar::aux::getUserApplicationDataDirectory() + "/.cedar/processingGui";
    try
    {
      this->writeJson(path);
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
      cedar::aux::LogSingleton::getInstance()->warning
      (
        std::string("Error saving framework gui settings: ") + e.what(),
        "void cedar::proc::gui::Settings::load()"
      );
    }
  }
}
