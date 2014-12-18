#include "stdafx.h"
#include "NotificationObserverForJobsManager.h"
#include <shared/Log.h>

namespace job
{

CNotificationObserverForJobsManager::CNotificationObserverForJobsManager(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
   :CThreadBase("NotificationObserverForJobsManager"), m_notificationCenter(notificationCenter)
{
   m_notificationCenter->registerObserver(&m_NotificationObserver);
   start();
}

CNotificationObserverForJobsManager::~CNotificationObserverForJobsManager()
{
   stop();
   m_notificationCenter->unregisterObserver(&m_NotificationObserver);
}

void CNotificationObserverForJobsManager::doWork()
{
   while (true)
   {
      switch(m_notificationCenter->waitForNotifications(&m_NotificationObserver))
      {
      case shared::notification::CNotificationCenter::kNotification:
         // Filter only newAcquisition notifications
         if (m_notificationCenter->isNotificationTypeOf< boost::shared_ptr<notifications::CNewAcquisitionNotification> >(&m_NotificationObserver))
            notifyListeners(m_notificationCenter->getNotificationData< boost::shared_ptr<notifications::CNewAcquisitionNotification> >(&m_NotificationObserver));
         break;
      default:
         YADOMS_LOG(error) << "Unexpected notification code";
         break;
      }
   }
}

void CNotificationObserverForJobsManager::notifyListeners(boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition)
{
   // For this keyword ID, notify each listeners, and each root condition (but just one time)

   std::set<boost::shared_ptr<condition::IConditionRootUpdater> > m_rootConditionsToNotify;

   std::pair<KeywordUpdaterList::iterator, KeywordUpdaterList::iterator> range = m_listeners.equal_range(newAcquisition->getAcquisition()->KeywordId);
   for (KeywordUpdaterList::iterator listener = range.first ; listener != range.second; ++listener)
   {
      KeywordUpdater& keywordUpdater = listener->second;

      // Notify keyword
      keywordUpdater.first->onKeywordStateChange(newAcquisition->getAcquisition()->Value);
      m_rootConditionsToNotify.insert(keywordUpdater.second);
   }

   // Now notify root conditions
   for (std::set<boost::shared_ptr<condition::IConditionRootUpdater> >::iterator condition = m_rootConditionsToNotify.begin();
      condition != m_rootConditionsToNotify.end() ; ++ condition)
  {
      (*condition)->onKeywordStateChange();
   }
}

void CNotificationObserverForJobsManager::registerKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier, boost::shared_ptr<condition::IConditionRootUpdater> conditionRootNotifier)
{
   m_listeners.insert(std::pair<int, KeywordUpdater>(keywordNotifier->getKeywordId(), KeywordUpdater(keywordNotifier, conditionRootNotifier)));
}

void CNotificationObserverForJobsManager::unregisterKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier)
{
   // Unregister the specified keyword updater, ands its associated root condition
   int keywordIdSearched = keywordNotifier->getKeywordId();
   for (KeywordUpdaterList::iterator listener = m_listeners.begin() ; listener != m_listeners.end() ; ++listener)
   {
      // The second test should be enough but add first test for optimisation
      if(listener->first == keywordIdSearched && listener->second.first == keywordNotifier)
      {
         // Iterator is now the item we looked for. Remove it now.
         m_listeners.erase(listener);
      }
   }
}


} // namespace job	
	
	