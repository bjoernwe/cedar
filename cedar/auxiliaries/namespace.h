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

    File:        namespace.h

    Maintainer:  Mathis Richter
    Email:       mathis.richter@ini.rub.de
    Date:        2010 10 12

    Description: Namespace file for cedar::aux.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_AUX_NAMESPACE_H
#define CEDAR_AUX_NAMESPACE_H

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "namespace.h"

// SYSTEM INCLUDES
#include <sstream>
#include <boost/smart_ptr.hpp>
#include <boost/property_tree/ptree.hpp>


namespace cedar
{
  /*!@brief Namespace for all aux classes. */
  namespace aux
  {
    class Base;
    //!@brief smart pointer for Base
    typedef boost::shared_ptr<Base> BasePtr;

    class Configurable;
    //!@brief smart pointer for Configurable
    typedef boost::shared_ptr<Configurable> ConfigurablePtr;

    class LoopedThread;
    //!@brief smart pointer for LoopedThread
    typedef boost::shared_ptr<LoopedThread> LoopedThreadPtr;

    class ConfigurationInterface;
    //!@brief smart pointer for ConfigurationInterface
    typedef boost::shared_ptr<ConfigurationInterface> ConfigurationInterfacePtr;

    class UserData;
    //!@brief smart pointer for UserData
    typedef boost::shared_ptr<UserData> UserDataPtr;

    template <typename T> class IntervalData;
    //!@brief smart pointer for IntervalData<double>
    typedef boost::shared_ptr<IntervalData<double> > DoubleIntervalPtr;
    //!@brief smart pointer for IntervalData<int>
    typedef boost::shared_ptr<IntervalData<int> > IntIntervalPtr;

    class LogFile;
    //!@brief smart pointer for LogFile
    typedef boost::shared_ptr<LogFile> LogFilePtr;

    class Object;
    //!@brief smart pointer for Object
    typedef boost::shared_ptr<Object> ObjectPtr;

    class Kernel;
    //!@brief smart pointer for Kernel
    typedef boost::shared_ptr<Kernel> KernelPtr;

    typedef unsigned int EnumId;
    class Enum;
    template <typename T> class EnumBase;

    class Arguments;

    template <class T> class Factory;
    template <class T, class T2> class FactoryDerived;

    template <class T> class AbstractFactory;
    template <class T, class T2> class AbstractFactoryDerived;

    template <class KeyBaseType, class ValueBaseType> class TypeBasedFactory;

    class ParameterBase;
    typedef boost::shared_ptr<ParameterBase> ParameterBasePtr;

    template <typename T> class Parameter;
    template <typename T> class NumericParameter;
    typedef NumericParameter<double> DoubleParameter;
    typedef boost::shared_ptr<DoubleParameter> DoubleParameterPtr;
    typedef NumericParameter<unsigned int> UIntParameter;
    typedef boost::shared_ptr<UIntParameter> UIntParameterPtr;
    typedef Parameter<std::string> StringParameter;
    typedef boost::shared_ptr<StringParameter> StringParameterPtr;
    typedef Parameter<bool> BoolParameter;
    typedef boost::shared_ptr<BoolParameter> BoolParameterPtr;

    typedef boost::property_tree::ptree ConfigurationNode;


    /*!@brief Template method that converts simple data types to a string.
     *
     * @param[in] value The data value that will be converted to a string.
     */
    template<typename T>
    std::string toString(const T &value)
    {
      std::ostringstream streamOut;
      streamOut << value;
      return streamOut.str();
    }

    /*exceptions*/
    class DuplicateNameException;
    class UnhandledTypeException;
    class UnknownTypeException;
    class ParameterNotFoundException;
  }
}

#endif // CEDAR_AUX_NAMESPACE_H
