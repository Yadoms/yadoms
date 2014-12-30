#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
#include "IConditionFactory.h"

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief The OR operator
   //-----------------------------------------------------
   class COr : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] operands Operands configuration
      ///\param[in] conditionFactory The condition factory
      //-----------------------------------------------------
      COr(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~COr();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForRulesManager> notificationObserver);
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The operand conditions
      //-----------------------------------------------------
      typedef std::vector<boost::shared_ptr<ICondition> > Operands;
      Operands m_operands;
   };
	
} } // namespace automation::condition	
	
	