#pragma once
#include "ICondition.h"
#include <shared/shared/DataContainer.h>
#include "IConditionFactory.h"

namespace job { namespace condition
{
   //-----------------------------------------------------
   ///\brief The AND operator
   //-----------------------------------------------------
   class CAnd : public ICondition
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] operands Operands configuration
      ///\param[in] conditionFactory The condition factory
      ///\param[in] conditionRoot The condition root to notify when changes arrives on a keyword of the condition
      ///\param[in] notificationObserver  The notification observer
      //-----------------------------------------------------
      CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory, IConditionRoot& conditionRoot, boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAnd();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The operand conditions
      //-----------------------------------------------------
      typedef std::vector<boost::shared_ptr<ICondition> > Operands;
      Operands m_operands;
   };
	
} } // namespace job::condition	
	
	