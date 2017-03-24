#include "stdafx.h"
#include "ProcessObserver.h"


CProcessObserver::CProcessObserver(int scriptInstanceId,
                                   boost::function3<void, bool, int, const std::string&> onInstanceStateChangedFct)
   : m_scriptInstanceId(scriptInstanceId),
     m_onInstanceStateChangedFct(onInstanceStateChangedFct)
{
}

CProcessObserver::~CProcessObserver()
{
}

void CProcessObserver::onStart()
{
   std::cout << "Script " << m_scriptInstanceId << " is running" << std::endl;
   m_onInstanceStateChangedFct(true,
                               m_scriptInstanceId,
                               std::string());
}

void CProcessObserver::onFinish(int returnCode,
                                const std::string& error)
{
   if (error.empty())
      std::cout << "Script " << m_scriptInstanceId << " is stopped" << std::endl;
   else
      std::cout << "Script " << m_scriptInstanceId << " is stopped with error (" << returnCode << ") : " << error << std::endl;

   m_onInstanceStateChangedFct(false,
                               m_scriptInstanceId,
                               error);
}

