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

    Maintainer:  Oliver Lomp,
                 Mathis Richter,
                 Stephan Zibner
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de,
                 mathis.richter@ini.ruhr-uni-bochum.de,
                 stephan.zibner@ini.ruhr-uni-bochum.de
    Date:        2011 06 03

    Description: Namespace file for cedar::dyn.

    Credits:

======================================================================================================================*/


#ifndef CEDAR_DYN_NAMESPACE_H
#define CEDAR_DYN_NAMESPACE_H

// LOCAL INCLUDES

// PROJECT INCLUDES
#include "namespace.h"

// SYSTEM INCLUDES
#include <boost/smart_ptr.hpp>
#include <opencv2/opencv.hpp>


namespace cedar
{
  /*!@brief Namespace for all dyn classes. */
  namespace dyn
  {
    class Dynamics;
    typedef boost::shared_ptr<Dynamics> DynamicsPtr;

    template <typename T> class Activation;
    typedef Activation<double> DoubleActivation;
    typedef boost::shared_ptr<DoubleActivation> DoubleActivationPtr;
    typedef Activation<cv::Mat> MatActivation;
    typedef boost::shared_ptr<MatActivation> MatActivationPtr;

    class SpaceCode;
    typedef boost::shared_ptr<SpaceCode> SpaceCodePtr;

    class NeuralField;
    typedef boost::shared_ptr<NeuralField> NeuralFieldPtr;

    void initialize();
  }
}

#endif // CEDAR_DYN_NAMESPACE_H
