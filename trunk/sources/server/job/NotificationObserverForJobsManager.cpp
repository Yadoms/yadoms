#include "stdafx.h"
#include "NotificationObserverForJobsManager.h"
#include <shared/Log.h>
#include "../notifications/NewAcquisitionNotification.h"

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
         {
            boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition = m_notificationCenter->getNotificationData< boost::shared_ptr<notifications::CNewAcquisitionNotification> >(&m_NotificationObserver);
            GlobalNotifierMap::const_iterator notifierListIt = m_notifiers.find(newAcquisition->getAcquisition()->KeywordId);
            if (notifierListIt != m_notifiers.end())
            {
               // This keyword ID is listen, notify all listeners
               boost::shared_ptr<NotifierList> notifierList(notifierListIt->second);
               for (NotifierList::const_iterator notifierIt = notifierList->begin(); notifierIt != notifierList->end(); ++notifierIt)
               {
                  (*notifierIt)->onNotify();                  
               }
            }            
         }
         break;
      default:
         YADOMS_LOG(error) << "Unexpected notification code";
         break;
      }
   }
}

void CNotificationObserverForJobsManager::registerKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier)
{
   //TODO ajouter un mutex là-dessus
   boost::shared_ptr<NotifierList>& notifierList = m_notifiers[keywordNotifier->getKeywordId()];
   if (!notifierList.get())
      notifierList.reset(new NotifierList);
   notifierList->insert(keywordNotifier);
}

void CNotificationObserverForJobsManager::unregisterKeyword(boost::shared_ptr<const condition::IKeywordNotifier> keywordNotifier)
{
   // Get the list of notifiers for this keywordId
   GlobalNotifierMap::const_iterator notifierListIt = m_notifiers.find(keywordNotifier->getKeywordId());
   BOOST_ASSERT_MSG(notifierListIt == m_notifiers.end(), "Keyword notifier was not registered");

   // Remove notifier from the list
   boost::shared_ptr<NotifierList> notifierList(notifierListIt->second);
   BOOST_ASSERT_MSG(notifierList->find(keywordNotifier) == notifierList->end(), "Keyword notifier was not registered for this keyword ID");

   notifierList->erase(keywordNotifier);

   // Remove the list if empty
   if (notifierList->empty())
      m_notifiers.erase(notifierListIt);
}


} // namespace job	
	
	