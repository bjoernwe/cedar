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

    File:        ConsoleLog.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 02 14

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/ConsoleLog.h"

// SYSTEM INCLUDES
#include <iostream>

// COLOR DEFINES
#ifdef __linux
#define COLOR_DARK_GRAY "1;30m"
#define COLOR_LIGHT_GRAY "0;37m"
#define COLOR_WHITE "1m"
#define COLOR_LIGHT_BLUE "1;34m"
#define COLOR_BLUE "0;34m"
#define COLOR_YELLOW "1;33m"
#define COLOR_GREEN "0;32m"
#define COLOR_BROWN "0;33m"
#define COLOR_RED "0;31m"
#define COLOR_LIGHT_RED "1;31m"
#define COLOR(COLOR_CONSTANT) "\033[" COLOR_CONSTANT
#define COLOR_CLEAR "\E[m"
#endif // __linux

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::aux::ConsoleLog::message(cedar::aux::LOG_LEVEL level, const std::string& message, const std::string& title)
{
  std::string formatted_message;
  
  if (!title.empty())
  {
    formatted_message += "[";
    formatted_message += title;
    formatted_message += "] ";
  }
  formatted_message += message;
  switch (level)
  {
    case cedar::aux::LOG_LEVEL_ERROR:
      std::cerr 
#ifdef __linux
                << COLOR(COLOR_RED)
#endif // __linux
                << "error> " << formatted_message
#ifdef __linux
                << COLOR_CLEAR
#endif // __linux
                << std::endl;
      break;
    
    case cedar::aux::LOG_LEVEL_WARNING:
      std::cout 
#ifdef __linux
                << COLOR(COLOR_BROWN)
#endif // __linux
                << "warning> "
                << formatted_message
#ifdef __linux
                << COLOR_CLEAR
#endif // __linux
                << std::endl;
      break;
      
    case cedar::aux::LOG_LEVEL_MESSAGE:
      std::cout 
                << "message> " << formatted_message
                << std::endl;
      break;
      
    case cedar::aux::LOG_LEVEL_SYSTEM_INFO:
      std::cout 
#ifdef __linux
                << COLOR(COLOR_DARK_GRAY)
#endif // __linux
                << "system info> "
                << formatted_message
#ifdef __linux
                << COLOR_CLEAR
#endif // __linux
                << std::endl;
      break;
      
    case cedar::aux::LOG_LEVEL_DEBUG:
      std::cout 
#ifdef __linux
                << COLOR(COLOR_BLUE)
#endif // __linux
                << "debug> " << formatted_message
#ifdef __linux
                << COLOR_CLEAR
#endif // __linux
                << std::endl;
      break;
      
    default:
      std::cout << "?> " << formatted_message << std::endl;
  }
}
  
