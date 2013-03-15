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

    File:        DeclarationManagerTemplate.h

    Maintainer:  Oliver Lomp
    Email:       oliver.lomp@ini.ruhr-uni-bochum.de
    Date:        2013 01 23

    Description:

    Credits:

======================================================================================================================*/

#ifndef CEDAR_AUX_DECLARATION_MANAGER_TEMPLATE_H
#define CEDAR_AUX_DECLARATION_MANAGER_TEMPLATE_H

// CEDAR CONFIGURATION
#include "cedar/configuration.h"

// CEDAR INCLUDES
#include "cedar/auxiliaries/namespace.h"
#include "cedar/auxiliaries/FactoryManager.h"

// SYSTEM INCLUDES


/*!@todo describe.
 *
 * @todo describe more.
 */
template <class BaseTypePtr>
class cedar::aux::DeclarationManagerTemplate
{
  // this class is a singleton
  friend class cedar::aux::Singleton<cedar::aux::DeclarationManagerTemplate<BaseTypePtr> >;

  //--------------------------------------------------------------------------------------------------------------------
  // nested types
  //--------------------------------------------------------------------------------------------------------------------
private:
  typedef cedar::aux::FactoryManager<BaseTypePtr> BaseFactoryManager;
  typedef cedar::aux::Singleton<BaseFactoryManager> BaseFactoryManagerSingleton;

public:
  typedef cedar::aux::PluginDeclarationBaseTemplate<BaseTypePtr> BasePluginDeclaration;
  CEDAR_GENERATE_POINTER_TYPES(BasePluginDeclaration);
  typedef std::vector<ConstBasePluginDeclarationPtr> BasePluginList;

  //--------------------------------------------------------------------------------------------------------------------
  // constructors and destructor
  //--------------------------------------------------------------------------------------------------------------------
private:
  //!@brief The standard constructor.
  DeclarationManagerTemplate()
  {
  }

  //--------------------------------------------------------------------------------------------------------------------
  // public methods
  //--------------------------------------------------------------------------------------------------------------------
public:
  void addDeclaration(ConstBasePluginDeclarationPtr declaration)
  {
    mDeclarations.push_back(declaration);

    const std::string& category = declaration->getCategory();
    if (mDeclarationsByCategory.find(category) == mDeclarationsByCategory.end())
    {
      mDeclarationsByCategory[category] = BasePluginList();
    }

    mDeclarationsByCategory[category].push_back(declaration);
  }

  boost::shared_ptr<BaseFactoryManager> getFactoryManager() const
  {
    return BaseFactoryManagerSingleton::getInstance();
  }

  BaseTypePtr allocate(const std::string& className) const
  {
    return this->getFactoryManager()->allocate(className);
  }

  std::string getTypeId(BaseTypePtr object) const
  {
    return this->getFactoryManager()->getTypeId(object);
  }

  cedar::aux::ConstPluginDeclarationPtr getDeclarationOf(BaseTypePtr object) const
  {
    for (size_t i = 0; i < mDeclarations.size(); ++i)
    {
      if (this->mDeclarations.at(i)->isObjectInstanceOf(object))
      {
        return this->mDeclarations.at(i);
      }
    }

    CEDAR_THROW(cedar::aux::UnknownTypeException, "Could not find the declaration for the given object.");
  }

  std::set<std::string> listCategories() const
  {
    std::set<std::string> result;
    for
    (
      typename std::map<std::string, BasePluginList>::const_iterator iter = this->mDeclarationsByCategory.begin();
      iter != this->mDeclarationsByCategory.end();
      ++iter
    )
    {
      result.insert(iter->first);
    }
    return result;
  }

  const BasePluginList& getCategoryEntries(const std::string& categoryName) const
  {
    typename std::map<std::string, BasePluginList>::const_iterator iter
      = this->mDeclarationsByCategory.find(categoryName);
    if (iter == this->mDeclarationsByCategory.end())
    {
      CEDAR_THROW(cedar::aux::UnknownNameException, "Category \"" + categoryName + " is not known.");
    }
    return iter->second;
  }

  const BasePluginList& getDeclarations() const
  {
    return this->mDeclarations;
  }

  //--------------------------------------------------------------------------------------------------------------------
  // protected methods
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // private methods
  //--------------------------------------------------------------------------------------------------------------------
private:
  // none yet

  //--------------------------------------------------------------------------------------------------------------------
  // members
  //--------------------------------------------------------------------------------------------------------------------
protected:
  // none yet
private:
  BasePluginList mDeclarations;

  std::map<std::string, BasePluginList> mDeclarationsByCategory;

}; // class cedar::aux::DeclarationManagerTemplate

#endif // CEDAR_AUX_DECLARATION_MANAGER_TEMPLATE_H

