#include "stdafx.h"
#include "YadomsCPULoad.h"
#include <shared/exception/Exception.hpp>

CYadomsCPULoad::CYadomsCPULoad(const std::string & device)
   :m_device(device), 
    m_keyword(new yApi::historization::CLoad("YadomsCPULoad")), 
    m_lastCPU(0), 
    m_lastSysCPU(0), 
    m_lastUserCPU(0), 
    m_numProcessors(0), 
    m_InitializeOk(false)
{
   Initialize();
}

void CYadomsCPULoad::Initialize()
{
   SYSTEM_INFO sysInfo;
   FILETIME ftime, fsys, fuser;

   GetSystemInfo(&sysInfo);
   m_numProcessors = sysInfo.dwNumberOfProcessors;

   GetSystemTimeAsFileTime(&ftime);
   memcpy(&m_lastCPU, &ftime, sizeof(FILETIME));

   m_self = GetCurrentProcess();
   if (!GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser))
   {
      std::stringstream Message;
      Message << "Fail to retrieve Process Times with the error :";
      Message << GetLastError();
      m_InitializeOk = false;
      throw shared::exception::CException ( Message.str() );
   }
   memcpy(&m_lastSysCPU, &fsys, sizeof(FILETIME));
   memcpy(&m_lastUserCPU, &fuser, sizeof(FILETIME));

   m_InitializeOk = true;
}

CYadomsCPULoad::~CYadomsCPULoad()
{
}

void CYadomsCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> api, shared::CDataContainer details)
{
   if (m_InitializeOk)
   {
      if (!api->keywordExists( m_device, m_keyword->getKeyword()))
        api->declareKeyword(m_device, *m_keyword, details);
   }
}

void CYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   if (!api)
      throw shared::exception::CException("api must be defined");

   if (m_InitializeOk)
   {
      api->historizeData(m_device, *m_keyword);
   }
}

void CYadomsCPULoad::read()
{
   FILETIME ftime, fsys, fuser;
   unsigned __int64 now, sys, user;

   if (m_InitializeOk)
   {
      GetSystemTimeAsFileTime(&ftime);
      memcpy(&now, &ftime, sizeof(FILETIME));

      if (!GetProcessTimes(m_self, &ftime, &ftime, &fsys, &fuser))
      {
         std::stringstream Message;
         Message << "Fail to retrieve Process Times with the error :";
         Message << GetLastError();
         throw shared::exception::CException ( Message.str() );
      }

      memcpy(&sys, &fsys, sizeof(FILETIME));
      memcpy(&user, &fuser, sizeof(FILETIME));
      double percent = double((sys - m_lastSysCPU) + (user - m_lastUserCPU)) / (now - m_lastCPU);
      percent /= m_numProcessors;
      m_lastCPU = now;
      m_lastUserCPU = user;
      m_lastSysCPU = sys;

	  float YadomsCPULoad = (float) floor((percent * 100)*10 + 0.5) /10;

      m_keyword->set( YadomsCPULoad );

      std::cout << "Yadoms CPU Load : " << m_keyword->formatValue() << std::endl;
   }
   else
   {
      std::cout << m_device << " is desactivated" << std::endl;
   }
}

boost::shared_ptr<yApi::historization::IHistorizable> CYadomsCPULoad::GetHistorizable() const
{
	return m_keyword;
}
