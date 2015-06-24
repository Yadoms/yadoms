#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>
#include "IConditionFactory.h"
#include "IConditionRoot.h"
#include "IConditionRootUpdater.h"
#include "../INotificationObserverForRulesManager.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The AND operator
   //-----------------------------------------------------
   class CConditionRoot : public IConditionRoot, public IConditionRootUpdater, public boost::enable_shared_from_this<CConditionRoot>
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] conditionConfiguration Conditions configuration
      ///\param[in] conditionFactory The condition factory
      //-----------------------------------------------------
      CConditionRoot(const shared::CDataContainer& conditionConfiguration, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CConditionRoot();

   protected:
      // IConditionRoot Implementation
      virtual void wait();
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] IConditionRoot Implementation

      // IConditionRootUpdater Implementation
      virtual void requestEvaluation();
      // [END] IConditionRootUpdater Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The main condition
      //-----------------------------------------------------
      boost::shared_ptr<ICondition> m_condition;

      //-----------------------------------------------------
      ///\brief               The event handler
      //-----------------------------------------------------
      shared::event::CEventHandler m_eventHandler;
   };
	
} } // namespace automation::condition	
	
	