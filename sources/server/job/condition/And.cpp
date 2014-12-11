#include "stdafx.h"
#include "And.h"
#include "ConditionFactory.h"

namespace job { namespace condition
{

CAnd::CAnd(const std::vector<shared::CDataContainer>& operands, const IConditionFactory& conditionFactory)
{
   for (std::vector<shared::CDataContainer>::const_iterator it = operands.begin(); it != operands.end(); ++it)
      m_operands.push_back(conditionFactory.createCondition(*it));      
}

CAnd::~CAnd()
{         
}

void CAnd::wait() const
{
   //TODO
}

} } // namespace job::condition	
	
	