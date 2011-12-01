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

    File:        StringFunctions.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2011 11 21

    Description: Advanced string functions.

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_STRING_FUNCTIONS_H
#define CEDAR_AUX_STRING_FUNCTIONS_H

// LOCAL INCLUDES
#include "cedar/auxiliaries/lib.h"

// SYSTEM INCLUDES
#include <vector>
#include <string>
#include <sstream>

namespace cedar
{
  namespace aux
  {
    /*!@brief Template method that converts simple data types to a string.
     *
     * @param value The data value that will be converted to a string.
     */
    template<typename T>
    std::string toString(const T &value)
    {
      std::ostringstream streamOut;
      streamOut << value;
      return streamOut.str();
    }

    /*!@brief  Splits a string based on the given separator.
     *
     *         For example, the string "This is an example" is split into the list ["This", "is", "an", "example"] when
     *         the separator " " is used.
     *
     * @param  str The string to split.
     * @param  separator The separator.
     * @param  parts Result vector into which the separated parts of the string are written.
     *
     * @remark If the separator is not found in \em str (e.g., if \em str is empty), the function will return a list
     *         with the input string as the only element.
     */
    inline void split(const std::string& str,
                      const std::string& separator,
                      std::vector<std::string>& parts)
    {
      parts.clear();

      size_t last_index = 0;
      size_t index = str.find(separator);
      while (index != std::string::npos)
      {
        std::string chunk = str.substr(last_index, index - last_index);
        parts.push_back(chunk);
        last_index = index + 1;
        index = str.find(separator, last_index);
      }
      std::string chunk = str.substr(last_index, index - last_index);
      parts.push_back(chunk);
    }

    /*!@brief Replaces all occurrences of needle in a given string with a specified replacement.
     *
     */
    inline std::string replace(const std::string& haystack, const std::string& needle, const std::string& replacement)
    {
      std::string result = haystack;

      if (haystack.empty())
      {
        return haystack;
      }

      size_t index = haystack.find(needle);
      while (index != std::string::npos)
      {
        result = result.replace(index, needle.length(), replacement);
        index = result.find(needle, index + replacement.length());
      }
      
      return result;
    }

    /*!@brief Template function that converts an STL string to a simple data type.
     *
     * @param value The data value that will be converted to a string.
     * @param string The string the value will be extracted from.
     * @param encoding The encoding of the string (e.g., the base of the number (i.e., decimal, hexadecimal, etc.))
     *        (see http://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-3.3/classstd_1_1ios__base.html)
     */
    template <class T>
    bool fromString(T& value, const std::string& string, std::ios_base& (*encoding)(std::ios_base&))
    {
      std::istringstream stream(string);
      return !(stream >> encoding >> value).fail();
    }
  }
}

#endif // CEDAR_AUX_STRING_FUNCTIONS_H
