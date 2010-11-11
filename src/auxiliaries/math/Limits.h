/*----------------------------------------------------------------------------------------------------------------------
 ----- Institute:   Ruhr-Universitaet Bochum
                    Institut fuer Neuroinformatik

 ----- File:        Limits.h

 ----- Author:      Mathis Richter
 ----- Email:       mathis.richter@ini.rub.de
 ----- Date:        2010 11 11

 ----- Description: Header for the \em cedar::aux::math::Limits struct.

 ----- Credits:
 ---------------------------------------------------------------------------------------------------------------------*/

#ifndef CEDAR_AUX_MATH_LIMITS_H
#define CEDAR_AUX_MATH_LIMITS_H

// LOCAL INCLUDES
#include "Namespace.h"

// PROJECT INCLUDES

// SYSTEM INCLUDES


/*!@brief Abstract description of the struct.
 *
 * More detailed description of the struct.
 */
template <class T_MIN, class T_MAX>
struct cedar::aux::math::Limits
{
  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
  //!@brief The standard constructor
  Limits();
  //!@brief Constructor that takes a minimum and maximum value
  Limits(const T_MIN& newMin, const T_MAX& newMax) : min(newMin), max(newMax) {};
  
  //!@brief Copy constructor
  template <class U, class V>
  Limits(const Limits<U, V> &otherLimits) : min(otherLimits.min), max(otherLimits.max) {};

  //--------------------------------------------------------------------------------------------------------------------
  // methods
  //--------------------------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
  //! minimum limit
  T_MIN min;
  //! maximum limit
  T_MAX max;

}; // class cedar::aux::math::Limits

#endif // CEDAR_AUX_MATH_LIMITS_H

