/*=============================================================================

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

===============================================================================

    Institute:   Ruhr-Universitaet Bochum
                 Institut fuer Neuroinformatik

    File:        AbstractNetWriter.h

    Maintainer:  Jean-Stephane Jokeit
    Email:       jean-stephane.jokeit@ini.ruhr-uni-bochum.de
    Date:        Tue 19 Jul 2011 03:15:20 PM CEST

    Description:

    Credits:

=============================================================================*/

#ifndef CEDAR_ABSTRACTNETWRITER_H
#define CEDAR_ABSTRACTNETWRITER_H

// LOCAL INCLUDES
#include "../namespace.h"
#include "AbstractNetBase.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <iostream>


using namespace std;

namespace _NM_CEDAR_ {
  namespace _NM_AUX_ {
    namespace _NM_NET_ {
      namespace _NM_INTERNAL_ {

/*!@brief Abstract Writer. function write() is virtual, handles basic connection
 *
 * Abstract Writer functionality (write datatype over network, RIIA)
 * Most of AbstractNetWriters functionality resides in AbstractNetBase.
 * write() is virtual and needs to be implemented in the child.
 */
template <typename T>
class AbstractNetWriter : public AbstractNetBase
{

  //---------------------------------------------------------------------------
  // constructors and destructor
  //---------------------------------------------------------------------------
private:
  //!@brief The standard constructor. Dont use
  AbstractNetWriter();
  AbstractNetWriter(const AbstractNetWriter &A); // not copyable
  AbstractNetWriter &operator=(const AbstractNetWriter &A);



public:
#define WRITER_PORT_NAME(x) ( (x) + PORT_DELIMINATOR + PORT_SUFFIX_OUT )
  //!@brief Use this constructor. Pass the user-defined port name as argument.
  //        The effective port name will be different.
  explicit AbstractNetWriter(const string &myPortName)  
                   : AbstractNetBase( WRITER_PORT_NAME(myPortName) )
  {
#ifdef DEBUG
    cout << "  AbstractNetWriter [CONSTRUCTOR]" << endl;
#endif
  }

  //!@brief Destructor virtual (to be sure)
  virtual ~AbstractNetWriter()
  {
#ifdef DEBUG
    cout << "  ~AbstractNetWriter [DESTRUCTOR]" << endl;
#endif
  }

  //---------------------------------------------------------------------------
  // protected methods
  //---------------------------------------------------------------------------
protected:


  //---------------------------------------------------------------------------
  // private methods
  //---------------------------------------------------------------------------
private:


  //---------------------------------------------------------------------------
  // public methods
  //---------------------------------------------------------------------------
public:

  //!@brief virtual write() needs to be implemented in child
  virtual void write(const T &t) = 0;
};

} } } }  // end namespaces

#endif
