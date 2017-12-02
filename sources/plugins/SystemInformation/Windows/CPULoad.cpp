#include "stdafx.h"
#include "CPULoad.h"
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "Helpers.h"

CCPULoad::CCPULoad(const std::string& keywordName)
   : m_keyword(boost::make_shared<yApi::historization::CLoad>(keywordName))
{
   readSystemValues(&ul_sys_idle_old, &ul_sys_kernel_old, &ul_sys_user_old);
}

CCPULoad::~CCPULoad()
{}

void CCPULoad::readSystemValues(ULARGE_INTEGER *sysIdle, ULARGE_INTEGER *sysKernel, ULARGE_INTEGER *sysUser)
{
   FILETIME               ft_sys_idle;
   FILETIME               ft_sys_kernel;
   FILETIME               ft_sys_user;

   if (!GetSystemTimes(&ft_sys_idle, &ft_sys_kernel, &ft_sys_user))
   {
      std::stringstream Message;
      Message << "GetSystemTimes failed with status:";
      Message << std::hex << GetLastError();
      shared::exception::CException(Message.str());
   }

   CopyMemory(sysIdle, &ft_sys_idle, sizeof(FILETIME)); // Could been optimized away...
   CopyMemory(sysKernel, &ft_sys_kernel, sizeof(FILETIME)); // Could been optimized away...
   CopyMemory(sysUser, &ft_sys_user, sizeof(FILETIME)); // Could been optimized away...
}

float  CCPULoad::getCpuUsage()
{
   ULARGE_INTEGER         ul_sys_idle;
   ULARGE_INTEGER         ul_sys_kernel;
   ULARGE_INTEGER         ul_sys_user;

   float  usage = 0;

   readSystemValues(&ul_sys_idle, &ul_sys_kernel, &ul_sys_user);

   ULONGLONG sysKernelDiff = (ul_sys_kernel.QuadPart - ul_sys_kernel_old.QuadPart);
   ULONGLONG sysUserDiff   = (ul_sys_user.QuadPart - ul_sys_user_old.QuadPart);
   ULONGLONG sysIdleDiff = (ul_sys_idle.QuadPart - ul_sys_idle_old.QuadPart);

   ul_sys_idle_old.QuadPart = ul_sys_idle.QuadPart;
   ul_sys_user_old.QuadPart = ul_sys_user.QuadPart;
   ul_sys_kernel_old.QuadPart = ul_sys_kernel.QuadPart;

   if ((sysKernelDiff + sysUserDiff) > 0)
      usage = ((sysKernelDiff + sysUserDiff - sysIdleDiff) * 100) / float(sysKernelDiff + sysUserDiff);
   else
      shared::exception::CException("time too short between two executions");

   return usage;
}

void CCPULoad::read()
{
   try
   {
      m_keyword->set(valueRoundWithPrecision(getCpuUsage(),3));
      YADOMS_LOG(trace) << "CPU Load : " << m_keyword->get() ;
   }
   catch (shared::exception::CException& exception)
   {
      YADOMS_LOG(warning) << "CPU Load reading failed:" << exception.what() ;
   }
}