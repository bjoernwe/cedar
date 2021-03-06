// LOCAL INCLUDES
#include "LinearDynamics.h"
#include "cedar/processing/ElementDeclaration.h"
#include <math.h>
#include <iostream>

//----------------------------------------------------------------------------------------------------------------------
// register the class
//----------------------------------------------------------------------------------------------------------------------
namespace
{
  bool declare()
  {
    using cedar::proc::ElementDeclarationPtr;
    using cedar::proc::ElementDeclarationTemplate;

    ElementDeclarationPtr declaration
    (
      new ElementDeclarationTemplate<cedar::proc::steps::LinearDynamics>
      (
        "Robotics",
        "cedar.processing.steps.LinearDynamics"
      )
    );
    declaration->setIconPath(":/steps/linear_dynamics.svg");
    declaration->setDescription
    (
      "Linear dynamics"
    );

    declaration->declare();

    return true;
  }

  bool declared = declare();
}


//----------------------------------------------------------------------------------------------------------------------
// constructors and destructor
//----------------------------------------------------------------------------------------------------------------------

cedar::proc::steps::LinearDynamics::LinearDynamics()
  :
  cedar::proc::Step(true),
  mpAcceleration(new cedar::aux::MatData(cv::Mat::zeros(1, 1, CV_32F))),
  mpLambda(new cedar::aux::DoubleParameter(this,"lambda", 0.0)),
  mpSDes(new cedar::aux::DoubleParameter(this, "fixed point", 0.0))
{
  this->declareInput("speed");
  this->declareOutput("acceleration", mpAcceleration);
}

//----------------------------------------------------------------------------------------------------------------------
// methods
//----------------------------------------------------------------------------------------------------------------------

void cedar::proc::steps::LinearDynamics::compute(const cedar::proc::Arguments&)
{
  const float &s = mpSpeed->getData().at<float>(0, 0);
  const float &s_des = mpSDes->getValue();
  const float &lambda = mpLambda->getValue();

  const float acceleration = lambda * (s - s_des); //dynamics

  mpAcceleration->getData().at<float>(0, 0) = acceleration;
}

//// validity check
cedar::proc::DataSlot::VALIDITY cedar::proc::steps::LinearDynamics::determineInputValidity
  (
   cedar::proc::ConstDataSlotPtr slot,
   cedar::aux::ConstDataPtr data
  ) const
{
  //all inputs have same type
  cedar::aux::ConstMatDataPtr _input = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(data);
  if( slot->getName() == "speed")
  {
    if (_input && _input->getDimensionality() == 0 && _input->getData().type() == CV_32F)
    {
      return cedar::proc::DataSlot::VALIDITY_VALID;
    }
  }
  
  // else
  return cedar::proc::DataSlot::VALIDITY_ERROR;
}

void cedar::proc::steps::LinearDynamics::inputConnectionChanged(const std::string& inputName)
{
  if (inputName == "speed")
  {
    mpSpeed = boost::dynamic_pointer_cast<cedar::aux::ConstMatData>(this->getInput(inputName));
  }
}
