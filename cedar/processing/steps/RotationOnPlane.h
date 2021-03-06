/*======================================================================================================================

    Copyright 2011, 2012 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany

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

    File:        RotationOnPlane.cpp

    Maintainer:  Nico Kuerschner
    Email:       Nico.Kuerschner@ini.ruhr-uni-bochum.de
    Date:        2017 January 25th

    Description: Takes two vectors and computes the angle, the plane normal vector and the scaled vector of most directional influence

    Credits:

======================================================================================================================*/

#ifndef ROTATION_ON_PLANE_H_
#define ROTATION_ON_PLANE_H_

#include <cedar/processing/Step.h>
#include <cedar/auxiliaries/MatData.h>
#include "cedar/processing/steps/RotationOnPlane.fwd.h"

class cedar::proc::steps::RotationOnPlane : public cedar::proc::Step
{
  Q_OBJECT
  public:
    RotationOnPlane();
    cedar::proc::DataSlot::VALIDITY determineInputValidity
                                    (
                                      cedar::proc::ConstDataSlotPtr slot,
                                      cedar::aux::ConstDataPtr data
                                    ) const;

  private:
    void compute(const cedar::proc::Arguments&);
    void inputConnectionChanged(const std::string& inputName);

    // input
    cedar::aux::ConstMatDataPtr mpEndeffectorVelocity;
    cedar::aux::ConstMatDataPtr mpEndeffectorPosition;
    cedar::aux::ConstMatDataPtr mpTargetPosition;

    // output
    cedar::aux::MatDataPtr mpAngle;
    cedar::aux::MatDataPtr mpOrthogonalAcceleration;
};

#endif /* ROTATION_ON_PLANE_H_ */
