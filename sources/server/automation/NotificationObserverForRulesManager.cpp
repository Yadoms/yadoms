#include "stdafx.h"
#include "NotificationObserverForRulesManager.h"
#include <shared/Log.h>

namespace automation
{

CNotificationObserverForRulesManager::CNotificationObserverForRulesManager(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter)
   :CThreadBase("NotificationObserverForRulesManager"), m_notificationCenter(notificationCenter)
{
   m_notificationCenter->registerObserver(&m_NotificationObserver);
   start();
}

CNotificationObserverForRulesManager::~CNotificationObserverForRulesManager()
{
   stop();
   m_notificationCenter->unregisterObserver(&m_NotificationObserver);
}

void CNotificationObserverForRulesManager::doWork()
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

void CNotificationObserverForRulesManager::notifyListeners(boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition)
{
   // For this keyword ID, notify each listeners, and each root condition (but just one time)

   boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

   std::queue<boost::shared_ptr<condition::IConditionRootUpdater> > m_rootConditionsQueue;
   std::set<boost::shared_ptr<condition::IConditionRootUpdater> > m_rootConditionsSet; // Used to disable duplicates

   std::pair<KeywordUpdaterList::iterator, KeywordUpdaterList::iterator> range = m_listeners.equal_range(newAcquisition->getAcquisition()->KeywordId);
   for (KeywordUpdaterList::iterator listener = range.first ; listener != range.second; ++listener)
   {
      KeywordUpdater& keywordUpdater = listener->second;

      // Notify keyword
      keywordUpdater.first->onKeywordStateChange(newAcquisition->getAcquisition()->Value);
      if (m_rootConditionsSet.find(keywordUpdater.second) == m_rootConditionsSet.end())
      {
         m_rootConditionsQueue.push(keywordUpdater.second);
         m_rootConditionsSet.insert(keywordUpdater.second);
      }
   }

   // Now notify root conditions
   while (!m_rootConditionsQueue.empty())
   {
      m_rootConditionsQueue.front()->onKeywordStateChange();
      m_rootConditionsQueue.pop();
   }
}

void CNotificationObserverForRulesManager::registerKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier, boost::shared_ptr<condition::IConditionRootUpdater> conditionRootNotifier)
{
   boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

   m_listeners.insert(std::pair<int, KeywordUpdater>(keywordNotifier->getKeywordId(), KeywordUpdater(keywordNotifier, conditionRootNotifier)));
}

void CNotificationObserverForRulesManager::unregisterKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier)
{
   boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

   // Unregister the specified keyword updater, ands its associated root condition
   for (KeywordUpdaterList::iterator listener = m_listeners.begin(); listener != m_listeners.end(); ++listener)
   {
      if (listener->second.first == keywordNotifier)
      {
         m_listeners.erase(listener);
         break;
      }
   }
}


} // namespace automation	
	
	