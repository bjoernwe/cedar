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

    File:        FrameworkSettings.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 26

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "processing/FrameworkSettings.h"
#include "auxiliaries/SetParameter.h"
#include "auxiliaries/DirectoryParameter.h"
#include "auxiliaries/System.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <boost/property_tree/json_parser.hpp>

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::FrameworkSettings::FrameworkSettings()
:
cedar::aux::Configurable()
{
  cedar::aux::ConfigurablePtr plugin_group(new cedar::aux::Configurable());
  this->addConfigurableChild("plugin", plugin_group);

  std::set<std::string> default_plugin_include_directories;
  this->mPluginIncludeDirectories = cedar::aux::StringSetParameterPtr
                                    (
                                      new cedar::aux::StringSetParameter
                                      (
                                        "includeDirectories",
                                        default_plugin_include_directories
                                      )
                                    );
  plugin_group->registerParameter(this->mPluginIncludeDirectories);

  mPluginWorkspace = cedar::aux::DirectoryParameterPtr
                     (
                         new cedar::aux::DirectoryParameter
                         (
                           "workspace",
                            cedar::aux::System::getUserHomeDirectory() + "/src/"
                         )
                     );
  plugin_group->registerParameter(this->mPluginWorkspace);

  std::set<std::string> default_known_plugins;
  this->mKnownPlugins = cedar::aux::StringSetParameterPtr
                        (
                          new cedar::aux::StringSetParameter
                          (
                            "knownPlugins",
                            default_known_plugins
                          )
                        );
  plugin_group->registerParameter(this->mKnownPlugins);

  this->load();
}

cedar::proc::FrameworkSettings::~FrameworkSettings()
{
  this->save();
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::FrameworkSettings::addKnownPlugin(const std::string& file)
{
  std::string modified_path = file;
  std::string workspace_path = this->mPluginWorkspace->get().absolutePath().toStdString();
  if (modified_path.find(workspace_path) == 0)
  {
    modified_path = modified_path.substr(workspace_path.length());
  }

  if (modified_path.empty())
    return;

  if (modified_path.at(0) == '/')
  {
    modified_path = modified_path.substr(1);
  }

  this->mKnownPlugins->insert(modified_path);
}

void cedar::proc::FrameworkSettings::load()
{
  std::string path = cedar::aux::System::getUserApplicationDataDirectory() + "/.cedar/processingFramework";
  try
  {
    this->readJson(path);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    //!@todo proper signaling(?) of this message to the gui.
    std::cout << "Error reading framework settings: " << e.what() << std::endl;
  }
}

void cedar::proc::FrameworkSettings::save()
{
  std::string path = cedar::aux::System::getUserApplicationDataDirectory() + "/.cedar/processingFramework";
  try
  {
    this->saveJson(path);
  }
  catch (const boost::property_tree::json_parser::json_parser_error& e)
  {
    //!@todo proper signaling(?) of this message to the gui.
    std::cout << "Error saving framework settings: " << e.what() << std::endl;
  }
}
