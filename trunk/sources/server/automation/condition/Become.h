#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "../INotificationObserverForRulesManager.h"
#include "IConditionRoot.h"
#include "IKeywordUpdater.h"
#include "../../database/IAcquisitionRequester.h"
#include "Comparators.h"

namespace automation { namespace condition
{
      
   //-----------------------------------------------------
   ///\brief The Become operator (front detector)
   //-----------------------------------------------------
   class CBecome : public ICondition, public IKeywordUpdater, public boost::enable_shared_from_this<CBecome>
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] configuration Condition configuration node
      ///\param[in] dbAcquisitionRequester  Database acquisition requester
      ///\throw shared::exception::CEmptyResult if associated keyword is not found
      //-----------------------------------------------------
      CBecome(const shared::CDataContainer& configuration, boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CBecome();

      // ICondition Implementation
      virtual bool eval();
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation

      // IKeywordUpdater Implementation
      virtual int getKeywordId() const;
      virtual void onKeywordStateChange(const std::string& state);
      // [END] IKeywordUpdater Implementation

   protected:
      //-----------------------------------------------------
      ///\brief               Convert value to double to perform '<' or '>' comparisons
      //-----------------------------------------------------
      bool evalState() const;

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
   };
	
} } // namespace automation::condition	
	
	