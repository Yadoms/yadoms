#include "stdafx.h"
#include "OperatingSystem.h"
#include <unistd.h>
#include <sys/reboot.h>

namespace tools {

   const std::string COperatingSystem::m_name = "Linux";

   const std::string & COperatingSystem::getName()
   {
      return m_name;
   }

   bool COperatingSystem::shutdown(bool andRestart)
   {
      sync();
      return (reboot(andRestart ? RB_AUTOBOOT : RB_POWER_OFF) == 0);
   }
   
} //namespace tools
