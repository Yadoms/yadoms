#include "stdafx.h"
#include "RuleThread.h"

namespace automation
{

CRuleThread::CRuleThread(const std::string & ruleName, IRuleThreadHandler& handler)
   :CThreadBase("rule #" + ruleName), m_handler(handler)
{
}

CRuleThread::~CRuleThread()
{         
}

void CRuleThread::doWork()
{
   m_handler.doWork();
}


} // namespace automation	
	
	