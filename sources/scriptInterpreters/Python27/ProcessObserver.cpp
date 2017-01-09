#include "stdafx.h"
#include <shared/Log.h>
#include "ProcessObserver.h"


CProcessObserver::CProcessObserver(int scriptInstanceId,
                                   boost::function2<void, bool, int> onInstanceStateChangedFct)
   : m_scriptInstanceId(scriptInstanceId),
     m_onInstanceStateChangedFct(onInstanceStateChangedFct)
{
}

CProcessObserver::~CProcessObserver()
{
}

void CProcessObserver::onStart()
{
   YADOMS_LOG(information) << "Interpreter " << m_scriptInstanceId << " is running";
   m_onInstanceStateChangedFct(true,
                               m_scriptInstanceId);
}

void CProcessObserver::onFinish(int returnCode,
                                const std::string& error)
{
   if (error.empty())
   YADOMS_LOG(information) << "Interpreter " << m_scriptInstanceId << " is stopped";
   else
   YADOMS_LOG(information) << "Interpreter " << m_scriptInstanceId << " is stopped with error (" << returnCode << ") : " << error;

   m_onInstanceStateChangedFct(false,
                               m_scriptInstanceId);
}

