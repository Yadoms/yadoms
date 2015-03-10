#include "stdafx.h"
#include "OperatingSystem.h"
#include <unistd.h>
#include <sys/reboot.h>
#include <Poco/Environment.h>

namespace tools {
   bool COperatingSystem::shutdown(bool andRestart)
   {
      sync();
      return (reboot(andRestart ? RB_AUTOBOOT : RB_POWER_OFF) == 0);
   }
   
} //namespace tools
