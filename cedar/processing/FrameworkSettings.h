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

    File:        FrameworkSettings.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 26

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_PROC_FRAMEWORK_SETTINGS_H
#define CEDAR_PROC_FRAMEWORK_SETTINGS_H

// LOCAL INCLUDES
#include "processing/namespace.h"
#include "auxiliaries/Configurable.h"
#include "auxiliaries/DirectoryParameter.h"
#include "auxiliaries/SetParameter.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <set>


/*!@brief Abstract description of the class.
 *
 * More detailed description of the class.
 */
class cedar::proc::FrameworkSettings : public cedar::aux::Configurable
{
  //--------------------------------------------------------------------------------------------------------------------
  // macros
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  //!@brief The standard constructor.
  FrameworkSettings();

  //!@brief Destructor
  ~FrameworkSettings();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  void load();
  void save();

  void addKnownPlugin(const std::string& file);
  const std::set<std::string>& getKnownPlugins();

  const std::set<std::string>& getPluginDirectories();

  std::string getPluginWorkspace() const;

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
public:
  // none yet (hopefully never!)
protected:
  // none yet
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // parameters
  //--------------------------------------------------------------------------------------------------------------------
protected:
  cedar::aux::DirectoryParameterPtr mPluginWorkspace;
  cedar::aux::StringSetParameterPtr mPluginIncludeDirectories;
  cedar::aux::StringSetParameterPtr mKnownPlugins;

private:
  // none yet

}; // class cedar::proc::FrameworkSettings

#endif // CEDAR_PROC_FRAMEWORK_SETTINGS_H

