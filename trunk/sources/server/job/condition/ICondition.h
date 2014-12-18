#pragma once
#include <server/job/INotificationObserverForJobsManager.h>

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The condition interface
   //-----------------------------------------------------
   class ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~ICondition() {}

      //-----------------------------------------------------
      ///\brief               Evaluate the condition
      ///\return              Return the evaluation result
      //-----------------------------------------------------
      virtual bool eval() const = 0;

      //-----------------------------------------------------
      ///\brief               Register to notification center
      ///\param[in] notificationObserver  The notification observer
      ///\param[in] conditionRootNotifier The condition root notifier
      //-----------------------------------------------------
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier) = 0;

      //-----------------------------------------------------
      ///\brief               Unregister to notification center
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver) = 0;
   };
	
} } // namespace job::condition
	
	