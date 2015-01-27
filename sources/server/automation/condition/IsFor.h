#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "../INotificationObserverForRulesManager.h"
#include "IConditionRoot.h"
#include "IKeywordUpdater.h"
#include "../../database/IAcquisitionRequester.h"
#include "Comparators.h"
#include <shared/event/EventHandler.hpp>
#include "IsForThread.h"

namespace automation { namespace condition
{      
   //-----------------------------------------------------
   ///\brief The IS...FOR operator
   //-----------------------------------------------------
   class CIsFor : public ICondition, public IKeywordUpdater, public boost::enable_shared_from_this<CIsFor>
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Condition configuration node
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\throw shared::exception::CEmptyResult if associated keyword is not found
      //-----------------------------------------------------
      CIsFor(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CIsFor();

      // ICondition Implementation
      virtual bool eval();
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation

      // IKeywordUpdater Implementation
      virtual int getKeywordId() const;
      virtual void onKeywordStateChange(const std::string& state);
      // [END] IKeywordUpdater Implementation  

      // ITimerListener Implementation
      virtual void onTimer();
      // [END] ITimerListener Implementation

   protected:
      //-----------------------------------------------------
      ///\brief               Internal state evaluation
      //-----------------------------------------------------
      bool evalState() const;

      //-----------------------------------------------------
      ///\brief               Start the duration timer
      //-----------------------------------------------------
      void startTimer();

      //-----------------------------------------------------
      ///\brief               Stop the duration timer
      //-----------------------------------------------------
      void stopTimer();

      //-----------------------------------------------------
      ///\brief               Convert value to double to perform '<' or '>' comparisons
      //-----------------------------------------------------
      static double toDouble(const std::string& value);

   private:
      //-----------------------------------------------------
      ///\brief               Keyword ID
      //-----------------------------------------------------
      const int m_keywordId;

      //-----------------------------------------------------
      ///\brief               Operator
      //-----------------------------------------------------
      const EComparators m_operator;

      //-----------------------------------------------------
      ///\brief               Current value
      //-----------------------------------------------------
      std::string m_value;

      //-----------------------------------------------------
      ///\brief               Expected value
      //-----------------------------------------------------
      const std::string m_expectedValue;

      //-----------------------------------------------------
      ///\brief               Previous comparison state
      //-----------------------------------------------------
      bool m_previousState;

      //-----------------------------------------------------
      ///\brief               Met condition flag
      //-----------------------------------------------------
      bool m_conditionIsMet;

      //-----------------------------------------------------
      ///\brief               Duration
      //-----------------------------------------------------
      const boost::posix_time::time_duration m_duration;

      //-----------------------------------------------------
      ///\brief               The root condition notifier
      //-----------------------------------------------------
      boost::shared_ptr<IConditionRootUpdater> m_ConditionRootNotifier;

      //-----------------------------------------------------
      ///\brief               The timer thread
      //-----------------------------------------------------
      CIsForThread m_timerThread;
   };
	
} } // namespace automation::condition	
	
	