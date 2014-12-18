#pragma once
#include <shared/DataContainer.h>
#include "ICondition.h"
#include "../INotificationObserverForJobsManager.h"
#include "IConditionRoot.h"

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The condition factory interface
   //-----------------------------------------------------
   class IConditionFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IConditionFactory() {}

      //-----------------------------------------------------
      ///\brief               Create a condition or sub-condition
      ///\param[in] configuration Condition configuration node
      //-----------------------------------------------------
      virtual boost::shared_ptr<ICondition> createCondition(const shared::CDataContainer& configuration) const = 0;
   };
	
} } // namespace job::condition
	
	