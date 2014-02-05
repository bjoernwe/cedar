/*======================================================================================================================

    Copyright 2011, 2012, 2013 Institut fuer Neuroinformatik, Ruhr-Universitaet Bochum, Germany
 
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

    File:        ActionSequence.h

    Maintainer:  Christian Bodenstein
    Email:       christian.bodenstein@ini.ruhr-uni-bochum.de
    Date:        2014 01 22

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_proc_EXPERIMENT_ACTION_SEQUENCE_H
#define CEDAR_proc_EXPERIMENT_ACTION_SEQUENCE_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/NamedConfigurable.h"
#include "cedar/auxiliaries/StringParameter.h"
#include "cedar/auxiliaries/UIntParameter.h"
#include "cedar/processing/experiment/Action.h"
#include "cedar/processing/experiment/Condition.h"
#include "cedar/auxiliaries/ObjectListParameterTemplate.h"
#include "cedar/auxiliaries/ObjectParameterTemplate.h"

// FORWARD DECLARATIONS
#include "cedar/processing/experiment/ActionSequence.fwd.h"

// SYSTEM INCLUDES


/*!@brief
 */
class cedar::proc::experiment::ActionSequence : public cedar::aux::NamedConfigurable
{

public:
  //!@brief a parameter for action sequence objects
  typedef cedar::aux::ObjectListParameterTemplate<cedar::proc::experiment::Action> ActionListParameter;
  typedef cedar::aux::ObjectParameterTemplate<cedar::proc::experiment::Condition> ConditionParameter;

  //!@cond SKIPPED_DOCUMENTATION
  CEDAR_GENERATE_POINTER_TYPES_INTRUSIVE(ActionListParameter);
  CEDAR_GENERATE_POINTER_TYPES_INTRUSIVE(ConditionParameter);
  //!@endcond

private:

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
public:
  ActionSequence();
  //!@brief Destructor
  ~ActionSequence();

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  void addAction(cedar::proc::experiment::ActionPtr action);
  void setCondition(cedar::proc::experiment::ConditionPtr condition);
  std::vector<cedar::proc::experiment::ActionPtr> getActions();
  std::vector<cedar::proc::experiment::ConditionPtr> getConditions();


  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

private:
  ActionListParameterPtr _mActionSet;
  ConditionParameterPtr _mCondition;

}; // class cedar::proc::experiment::Experiment

#include "cedar/auxiliaries/FactoryManager.h"

CEDAR_AUX_EXPORT_SINGLETON(cedar::aux::FactoryManager<cedar::proc::experiment::ActionSequencePtr>);

namespace cedar
{
  namespace proc
  {
    namespace experiment
    {
      //!@brief The singleton instance of the kernel factory manager.
      typedef cedar::aux::Singleton< cedar::aux::FactoryManager<cedar::proc::experiment::ActionSequencePtr>>
              ActionSequenceManagerSingleton;
    }
  }
}

#endif // CEDAR_proc_EXPERIMENT_ACTION_SEQUENCE_H

