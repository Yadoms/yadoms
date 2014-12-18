#pragma once
#include "condition/IKeywordUpdater.h"
#include "condition/IConditionRootUpdater.h"

namespace job
{
   //-----------------------------------------------------
   ///\brief The notification observer for the job manager interface
   //-----------------------------------------------------
   class INotificationObserverForJobsManager
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~INotificationObserverForJobsManager() {}

      //-----------------------------------------------------
      ///\brief               Register keyword
      ///\param[in] keywordUpdater Keyword updater to register
      ///\param[in] conditionRootNotifier Condition root updater to register
      //-----------------------------------------------------
      virtual void registerKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordUpdater, boost::shared_ptr<condition::IConditionRootUpdater> conditionRootNotifier) = 0;

      //-----------------------------------------------------
      ///\brief               Unregister keyword
      ///\param[in] keywordUpdater Keyword updater to unregister
      //-----------------------------------------------------
      virtual void unregisterKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordUpdater) = 0;
   };
	
} // namespace job	
	
	