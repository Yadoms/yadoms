#pragma once
#include "condition/IKeywordNotifier.h"

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
      ///\param[in] keywordNotifier Keyword notifier to register
      //-----------------------------------------------------
      virtual void registerKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier) = 0;

      //-----------------------------------------------------
      ///\brief               Unregister keyword
      ///\param[in] keywordNotifier Keyword notifier to unregister
      //-----------------------------------------------------
      virtual void unregisterKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier) = 0;
   };
	
} // namespace job	
	
	