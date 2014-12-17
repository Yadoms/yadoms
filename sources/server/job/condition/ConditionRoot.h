#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include <shared/event/EventHandler.hpp>
#include "IConditionFactory.h"
#include "IConditionRoot.h"
#include "../INotificationObserverForJobsManager.h"

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The AND operator
   //-----------------------------------------------------
   class CConditionRoot : public IConditionRoot
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] conditionConfiguration Conditions configuration
      ///\param[in] notificationObserver  The notification observer
      ///\param[in] conditionFactory The condition factory
      //-----------------------------------------------------
      CConditionRoot(const shared::CDataContainer& conditionConfiguration, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CConditionRoot();

   protected:
      // IConditionRoot Implementation
      virtual void eval();
      virtual void wait();
      // [END] IConditionRoot Implementation

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
	
} } // namespace job::condition	
	
	