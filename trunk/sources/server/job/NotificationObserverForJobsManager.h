#pragma once
#include "INotificationObserverForJobsManager.h"
#include <shared/notification/NotificationCenter.h>
#include <shared/ThreadBase.h>

namespace job
{
   //-----------------------------------------------------
   ///\brief The notification observer for the job manager
   //-----------------------------------------------------
   class CNotificationObserverForJobsManager : public INotificationObserverForJobsManager, public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      //-----------------------------------------------------
      CNotificationObserverForJobsManager(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNotificationObserverForJobsManager();

      // INotificationObserverForJobsManager Implementation
      virtual void registerKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier);
      virtual void unregisterKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier);
      // [END] INotificationObserverForJobsManager Implementation

   protected:
      // CThreadBase implementation
      virtual void doWork();
      // [END] CThreadBase implementation

   private:
      //-----------------------------------------------------
      ///\brief               The notification center
      //-----------------------------------------------------
      boost::shared_ptr<shared::notification::CNotificationCenter> m_notificationCenter;

      //-----------------------------------------------------
      ///\brief               The notification center
      //-----------------------------------------------------
      shared::notification::CNotificationObserver m_NotificationObserver;

      //-----------------------------------------------------
      ///\brief               The list of listen keywords, sorted by keywordId
      //-----------------------------------------------------
      typedef std::set<boost::shared_ptr<const condition::IKeywordNotifier> > NotifierList; // List of notifiers for a keyword
      typedef std::map<int, boost::shared_ptr<NotifierList> > GlobalNotifierMap; // List of notifiers for all keywords (key is the keywordId)
      GlobalNotifierMap m_notifiers;
   };
	
} // namespace job	
	
	