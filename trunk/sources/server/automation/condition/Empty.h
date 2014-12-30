#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief A condition
   //-----------------------------------------------------
   class CEmpty : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CEmpty();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CEmpty();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation
   };
	
} } // namespace automation::condition	
	
	