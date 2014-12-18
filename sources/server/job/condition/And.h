#pragma once
#include "ICondition.h"
#include <shared/DataContainer.h>
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
      //-----------------------------------------------------
      CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAnd();

   protected:
      // ICondition Implementation
      virtual bool eval() const;
      virtual void registerToNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver, boost::shared_ptr<IConditionRootUpdater> conditionRootNotifier);
      virtual void unregisterFromNotificationCenter(boost::shared_ptr<INotificationObserverForJobsManager> notificationObserver);
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The operand conditions
      //-----------------------------------------------------
      typedef std::vector<boost::shared_ptr<ICondition> > Operands;
      Operands m_operands;
   };
	
} } // namespace job::condition	
	
	