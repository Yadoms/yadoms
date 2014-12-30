#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "IConditionFactory.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The NOT operator
   //-----------------------------------------------------
   class CNot : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] operand Operand configuration
      ///\param[in] conditionFactory The condition factory
      //-----------------------------------------------------
      CNot(const shared::CDataContainer& operand, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNot();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The operand condition
      //-----------------------------------------------------
      boost::shared_ptr<ICondition> m_operand;
   };
	
} } // namespace automation::condition	
	
	