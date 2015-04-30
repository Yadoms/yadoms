#include "stdafx.h"
#include "OperatingSystem.h"
#include <unistd.h>
#include <sys/reboot.h>
#include <Poco/Environment.h>
#include <Poco/StringTokenizer.h>

namespace tools {
   bool COperatingSystem::shutdown(bool andRestart)
   {
      sync();
      return (reboot(andRestart ? RB_AUTOBOOT : RB_POWER_OFF) == 0);
   }

   std::string COperatingSystem::getName()
   {
      return "linux";
   }
   
   CVersion COperatingSystem::getVersion()
   {
      // Poco::Environment::osVersion() returns version like "3.18.7-v7+"
      Poco::StringTokenizer tokenizer(Poco::Environment::osVersion(), ".-+");
      if (tokenizer.count() < 3)
         throw shared::exception::CException("Unable to get OS version");

      CVersion version(tokenizer[0], tokenizer[1], tokenizer[2]);
      return version;
   }
   
} //namespace tools
