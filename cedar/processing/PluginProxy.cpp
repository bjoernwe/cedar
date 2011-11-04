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

    File:        PluginProxy.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 07 22

    Description:

    Credits:

======================================================================================================================*/

// LOCAL INCLUDES
#include "cedar/processing/PluginProxy.h"
#include "cedar/processing/PluginDeclaration.h"
#include "cedar/processing/Manager.h"
#include "cedar/processing/exceptions.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#ifdef UNIX
#include <dlfcn.h>
#elif defined WINDOWS
#include <Windows.h>
#include <strsafe.h>
#endif

#include <boost/filesystem.hpp>


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::PluginProxy::PluginProxy()
:
mpLibHandle(NULL)
{
}

cedar::proc::PluginProxy::PluginProxy(const std::string& file)
:
mpLibHandle(NULL)
{
  this->load(file);
}

cedar::proc::PluginProxy::~PluginProxy()
{
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

std::string cedar::proc::PluginProxy::findPluginFile(const std::string& file)
{
  std::string searched_locs;

  searched_locs += file + "\n";
  if (boost::filesystem::exists(file))
  {
    return file;
  }

  cedar::proc::FrameworkSettings& settings = cedar::proc::Manager::getInstance().settings();
  std::string loc = settings.getPluginWorkspace() + "/" + file;
  searched_locs += loc;
  if (boost::filesystem::exists(loc))
  {
    return loc;
  }

  std::string ret_path;
  std::set<std::string>::const_iterator path = settings.getPluginDirectories().begin();
  std::set<std::string>::const_iterator path_end = settings.getPluginDirectories().end();
  if (path != path_end)
  {
    do
    {
      ret_path = (*path);
      if (path->size() > 0 && path->at(path->size() - 1) != '/')
      {
        ret_path += '/';
      }
      ret_path += file;
      searched_locs += ret_path + "\n";
      ++path;
    }
    while (!boost::filesystem::exists(ret_path) && path != path_end);
  }

  CEDAR_THROW(cedar::proc::PluginException, "Could not load plugin: file not found. Searched locations: " + searched_locs);

  return "";
}

void cedar::proc::PluginProxy::load(const std::string& file)
{
  this->mFileName = this->findPluginFile(file);
  
  // OS-Dependent code for loading the plugin.
  PluginInterfaceMethod p_interface = NULL;
#ifdef UNIX
  this->mpLibHandle = dlopen(this->mFileName.c_str(), RTLD_NOW);
  if (!this->mpLibHandle)
  {
    std::string dl_err = dlerror();
    CEDAR_THROW(cedar::proc::PluginException, "Could not load plugin: dlopen failed. dlerror() returned " + dl_err);
  }


  p_interface = (PluginInterfaceMethod) (dlsym(this->mpLibHandle, "pluginDeclaration"));
  if (!p_interface)
  {
    CEDAR_THROW(cedar::proc::PluginException, "Error loading interface function: dlsym returned NULL.");
  }

  //@todo this might segfault if the function pointer points to a bad function; handle this somehow.
#elif defined WINDOWS
  this->mpLibHandle = LoadLibraryEx(this->mFileName.c_str(), NULL, 0);
  if (!this->mpLibHandle)
  {
    //!@todo use GetLastError to read out the error string
    CEDAR_THROW(cedar::proc::PluginException, "Could not load plugin: LoadLibraryEx failed: " + this->getLastError());
  }
  
  p_interface = (PluginInterfaceMethod) (GetProcAddress(this->mpLibHandle, TEXT("pluginDeclaration")));
  if (!p_interface)
  {
    //!@todo use GetLastError to read out the error string
    CEDAR_THROW(cedar::proc::PluginException, "Error loading interface function: GetProcAddress failed: " + this->getLastError());
  }
#endif // LINUX / WINDOWS
  
  this->mDeclaration = cedar::proc::PluginDeclarationPtr(new cedar::proc::PluginDeclaration());
  (*p_interface)(this->mDeclaration);

  // Finally, if nothing failed, add the plugin to the list of known plugins.
  cedar::proc::Manager::getInstance().settings().addKnownPlugin(file);
}

cedar::proc::PluginDeclarationPtr cedar::proc::PluginProxy::getDeclaration()
{
  return this->mDeclaration;
}

#ifdef WINDOWS

std::string cedar::proc::PluginProxy::getLastError()
{
  LPVOID lpMsgBuf;
  DWORD dw = GetLastError(); 

  FormatMessage(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      dw,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPTSTR) &lpMsgBuf,
      0, NULL );
  
  std::string error((char*)lpMsgBuf);

  LocalFree(lpMsgBuf);
  return error;
}

#endif //def WINDOWS
