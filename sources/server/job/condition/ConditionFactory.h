#pragma once
#include "IConditionFactory.h"
#include <shared/shared/DataContainer.h>

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The condition factory
   //-----------------------------------------------------
   class CConditionFactory : public IConditionFactory
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CConditionFactory();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CConditionFactory();

   protected:
      // IConditionFactory Implementation
      virtual boost::shared_ptr<ICondition> createCondition(const shared::CDataContainer& configuration, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver) const;
      // [END] IConditionFactory Implementation
   };
	
} } // namespace job::condition	
	
	