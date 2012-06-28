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

    File:        IntVectorParameter.cpp

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2012 03 12

    Description:

    Credits:

======================================================================================================================*/

// CEDAR INCLUDES
#include "cedar/auxiliaries/gui/IntVectorParameter.h"
#include "cedar/auxiliaries/IntVectorParameter.h"
#include "cedar/auxiliaries/assert.h"
#include "cedar/auxiliaries/TypeBasedFactory.h"
#include "cedar/auxiliaries/Singleton.h"

// SYSTEM INCLUDES
#include <QVBoxLayout>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// associate aux::gui parameter with the aux parameter
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool registered = cedar::aux::gui::ParameterFactorySingleton::getInstance()->add
      <
        cedar::aux::IntVectorParameter,
        cedar::aux::gui::IntVectorParameter
      >();
}

//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::aux::gui::IntVectorParameter::IntVectorParameter(QWidget *pParent)
:
cedar::aux::gui::Parameter(pParent)
{
  this->setLayout(new QVBoxLayout());
  this->layout()->setContentsMargins(0, 0, 0, 0);
  QObject::connect(this, SIGNAL(parameterPointerChanged()), this, SLOT(parameterPointerChanged()));
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------


void cedar::aux::gui::IntVectorParameter::parameterPointerChanged()
{
  cedar::aux::IntVectorParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::aux::IntVectorParameter>(this->getParameter());

  QObject::connect(parameter.get(), SIGNAL(propertyChanged()), this, SLOT(propertyChanged()));
  this->propertyChanged();
}

void cedar::aux::gui::IntVectorParameter::propertyChanged()
{
  cedar::aux::IntVectorParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::aux::IntVectorParameter>(this->getParameter());
  //!@todo Don't throw away old spinboxes, reuse them instead
  // Create the appropriate amount of spinboxes
  if (this->mSpinboxes.size() != parameter->size())
  {
    for (size_t i = 0; i < this->mSpinboxes.size(); ++i)
    {
      delete this->mSpinboxes.at(i);
    }
    this->mSpinboxes.clear();

    parameter->lockForRead();
    for (size_t i = 0; i < parameter->size(); ++i)
    {
      QSpinBox *p_widget = new QSpinBox();
      this->mSpinboxes.push_back(p_widget);
      this->layout()->addWidget(p_widget);
      p_widget->setMinimumHeight(20);

      // the limits have to be set here already so the value is set properly.
      this->mSpinboxes.at(i)->setMinimum(parameter->getMinimum());
      this->mSpinboxes.at(i)->setMaximum(parameter->getMaximum());
      p_widget->setValue(parameter->at(i));
      QObject::connect(p_widget, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));
    }
    parameter->unlock();

    emit heightChanged();
  }

  // Update the spinboxes' properties
  parameter->lockForRead();
  for (size_t i = 0; i < this->mSpinboxes.size(); ++i)
  {
    this->mSpinboxes.at(i)->setMinimum(parameter->getMinimum());
    this->mSpinboxes.at(i)->setMaximum(parameter->getMaximum());
    this->mSpinboxes.at(i)->setEnabled(!parameter->isConstant());
  }
  parameter->unlock();
}

void cedar::aux::gui::IntVectorParameter::valueChanged(int)
{
  cedar::aux::IntVectorParameterPtr parameter;
  parameter = boost::dynamic_pointer_cast<cedar::aux::IntVectorParameter>(this->getParameter());
  std::vector<int> values = parameter->getValue();
  CEDAR_DEBUG_ASSERT(this->mSpinboxes.size() == values.size());
  for (size_t i = 0; i < values.size(); ++i)
  {
    values.at(i) = this->mSpinboxes.at(i)->value();
  }

  parameter->set(values, true);
}
