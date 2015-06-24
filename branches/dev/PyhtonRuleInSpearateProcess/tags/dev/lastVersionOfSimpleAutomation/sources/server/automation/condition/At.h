#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "IConditionRoot.h"
#include "AtThread.h"

namespace automation { namespace condition
{      
   //-----------------------------------------------------
   ///\brief The AT operator
   //-----------------------------------------------------
   class CAt : public ICondition, public boost::enable_shared_from_this<CAt>
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor                         
      ///\param[in] timePoint Time point
      //-----------------------------------------------------
      CAt(const boost::posix_time::ptime& timePoint);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAt();

      // ICondition Implementation
      virtual bool eval();
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation

      virtual void onTimer();

   protected:                          
      //-----------------------------------------------------
      ///\brief               Start the timer
      //-----------------------------------------------------
      void startTimer();

      //-----------------------------------------------------
      ///\brief               Stop the timer
      //-----------------------------------------------------
      void stopTimer();

   private:                         
      //-----------------------------------------------------
      ///\brief               Met condition flag
      //-----------------------------------------------------
      bool m_conditionIsMet;

      //-----------------------------------------------------
      ///\brief               The time point
      //-----------------------------------------------------
      boost::posix_time::ptime m_timePoint;

      //-----------------------------------------------------
      ///\brief               The root condition notifier
      //-----------------------------------------------------
      boost::shared_ptr<IConditionRootUpdater> m_ConditionRootNotifier;

      //-----------------------------------------------------
      ///\brief               The timer thread
      //-----------------------------------------------------
      CAtThread m_timerThread;
   };
	
} } // namespace automation::condition	
	
	