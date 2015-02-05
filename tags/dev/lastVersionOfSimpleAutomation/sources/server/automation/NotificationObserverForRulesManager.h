#pragma once
#include "INotificationObserverForRulesManager.h"
#include <shared/notification/NotificationCenter.h>
#include <shared/ThreadBase.h>
#include "../notifications/NewAcquisitionNotification.h"

namespace automation
{
   //-----------------------------------------------------
   ///\brief The notification observer for the rule manager
   //-----------------------------------------------------
   class CNotificationObserverForRulesManager : public INotificationObserverForRulesManager, public shared::CThreadBase
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] notificationCenter Notification center, used to get notified on keyword state changes
      //-----------------------------------------------------
      CNotificationObserverForRulesManager(boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNotificationObserverForRulesManager();

      // INotificationObserverForRulesManager Implementation
      virtual void registerKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier, boost::shared_ptr<condition::IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterKeywordUpdater(boost::shared_ptr<condition::IKeywordUpdater> keywordNotifier);
      // [END] INotificationObserverForRulesManager Implementation

   protected:
      // CThreadBase implementation
      virtual void doWork();
      // [END] CThreadBase implementation

      //-----------------------------------------------------
      ///\brief               Notify listeners of keyword state change
      ///\param[in] newAcquisition New acquisition data
      //-----------------------------------------------------
      void notifyListeners(boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition);

      //-----------------------------------------------------
      ///\brief               Notify keyword listeners of state change
      ///\param[in] newAcquisition New acquisition data
      ///\param[in,out] rootConditionsQueue Conditions root list, to notify later
      ///\note : This function is not thread-safe, mutex must be locked before call
      //-----------------------------------------------------
      void notifyKeywordListeners(boost::shared_ptr<notifications::CNewAcquisitionNotification> newAcquisition,
         std::queue<boost::shared_ptr<condition::IConditionRootUpdater> >& rootConditionsQueue);

      //-----------------------------------------------------
      ///\brief               Notify all conditions root
      ///\param[in] rootConditionsQueue Conditions root list
      ///\note : This function is not thread-safe, mutex must be locked before call
      //-----------------------------------------------------
      void notifyRootconditions(std::queue<boost::shared_ptr<condition::IConditionRootUpdater> >& rootConditionsQueue);

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
      typedef std::pair<boost::shared_ptr<condition::IKeywordUpdater>, boost::shared_ptr<condition::IConditionRootUpdater> > KeywordUpdater;
      typedef std::multimap<int, KeywordUpdater> KeywordUpdaterList;
      KeywordUpdaterList m_KeywordListeners;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the keyword list
      //--------------------------------------------------------------
      mutable boost::recursive_mutex m_listenersMutex;
   };
	
} // namespace automation	
	
	