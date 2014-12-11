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
      //-----------------------------------------------------
      CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CAnd();

   protected:
      // ICondition Implementation
      virtual void wait() const;
      // [END] ICondition Implementation

   private:
      //-----------------------------------------------------
      ///\brief               The operand conditions
      //-----------------------------------------------------
      std::vector<boost::shared_ptr<condition::ICondition> > m_operands;
   };
	
} } // namespace job::condition	
	
	