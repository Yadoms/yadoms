#include "stdafx.h"
#include "YadomsCPULoad.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>

CYadomsCPULoad::CYadomsCPULoad(const std::string & device)
   :m_device(device), m_keyword("YadomsCPULoad"), m_lastCPU(0), m_lastSysCPU(0), m_lastUserCPU(0), m_numProcessors(0), m_InitializeOk(false)
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

void CYadomsCPULoad::declareKeywords(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (m_InitializeOk)
   {
      if (!context->keywordExists( m_device, m_keyword.getKeyword()))
      {
         context->declareKeyword(m_device, m_keyword);
      }
   }
}

void CYadomsCPULoad::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   BOOST_ASSERT_MSG(context, "context must be defined");

   if (m_InitializeOk)
   {
      context->historizeData(m_device, m_keyword);
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

      m_keyword.set((float)(percent * 100));
      YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Yadoms CPU Load : " << m_keyword.formatValue();
   }
   else
   {
      YADOMS_LOG(trace) << m_device << " is desactivated";
   }
}
