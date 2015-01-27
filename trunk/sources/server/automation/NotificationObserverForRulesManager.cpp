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

   std::queue<boost::shared_ptr<condition::IConditionRootUpdater> > rootConditionsQueue;
   notifyKeywordListeners(newAcquisition, rootConditionsQueue);
   notifyRootconditions(rootConditionsQueue);
}

void CNotificationObserverForRulesManager::notifyKeywordListeners(
   boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition,
   std::queue<boost::shared_ptr<condition::IConditionRootUpdater> >& rootConditionsQueue)
{
   std::set<boost::shared_ptr<condition::IConditionRootUpdater> > rootConditionsSet; // Used to prevent duplicates

   std::pair<KeywordUpdaterList::iterator, KeywordUpdaterList::iterator> range = m_KeywordListeners.equal_range(newAcquisition->getAcquisition()->KeywordId);
   for (KeywordUpdaterList::iterator listener = range.first; listener != range.second; ++listener)
   {
      KeywordUpdater& keywordUpdater = listener->second;

      // Notify keyword
      keywordUpdater.first->onKeywordStateChange(newAcquisition->getAcquisition()->Value);
      if (rootConditionsSet.find(keywordUpdater.second) == rootConditionsSet.end())
      {
         rootConditionsQueue.push(keywordUpdater.second);
         rootConditionsSet.insert(keywordUpdater.second);
      }
   }
}

void CNotificationObserverForRulesManager::notifyRootconditions(std::queue<boost::shared_ptr<condition::IConditionRootUpdater> >& rootConditionsQueue)
{
   while (!rootConditionsQueue.empty())
   {
      rootConditionsQueue.front()->requestEvaluation();
      rootConditionsQueue.pop();
   }
}

void CNotificationObserverForRulesManager::registerKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier, boost::shared_ptr<condition::IConditionRootUpdater> conditionRootNotifier)
{
   boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

   m_KeywordListeners.insert(std::pair<int, KeywordUpdater>(keywordNotifier->getKeywordId(), KeywordUpdater(keywordNotifier, conditionRootNotifier)));
}

void CNotificationObserverForRulesManager::unregisterKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier)
{
   boost::recursive_mutex::scoped_lock lock(m_listenersMutex);

   // Unregister the specified keyword updater, ands its associated root condition
   for (KeywordUpdaterList::iterator listener = m_KeywordListeners.begin(); listener != m_KeywordListeners.end(); ++listener)
   {
      if (listener->second.first == keywordNotifier)
      {
         m_KeywordListeners.erase(listener);
         break;
      }
   }
}


} // namespace automation	
	
	