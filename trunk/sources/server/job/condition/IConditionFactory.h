#pragma once
#include <shared/shared/DataContainer.h>
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
      ///\param[in] conditionRoot The condition root to notify when changes arrives on a keyword of the condition
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      virtual boost::shared_ptr<ICondition> createCondition(const shared::CDataContainer& configuration, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver) const = 0;
   };
	
} } // namespace job::condition
	
	