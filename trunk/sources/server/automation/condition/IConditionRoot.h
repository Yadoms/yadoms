#pragma once
#include"../INotificationObserverForRulesManager.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The root condition interface
   //-----------------------------------------------------
   class IConditionRoot
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IConditionRoot() {}

      //-----------------------------------------------------
      ///\brief               Wait for condition is met
      //-----------------------------------------------------
      virtual void wait() = 0;

      //-----------------------------------------------------
      ///\brief               Register to notification center
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver) = 0;

      //-----------------------------------------------------
      ///\brief               Unregister to notification center
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver) = 0;
   };
	
} } // namespace automation::condition
	
	