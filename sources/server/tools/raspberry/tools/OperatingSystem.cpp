#include "stdafx.h"
#include "OperatingSystem.h"
#include <unistd.h>
#include <sys/reboot.h>
#include <Poco/Environment.h>
#include <Poco/StringTokenizer.h>
#include <shared/exception/Exception.hpp>

namespace tools {
   bool COperatingSystem::shutdown(bool andRestart)
   {
      sync();
      return (reboot(andRestart ? RB_AUTOBOOT : RB_POWER_OFF) == 0);
   }

   std::string COperatingSystem::getName()
   {
      return "raspberry";
   }
   
   shared::versioning::CVersion COperatingSystem::getVersion()
   {
      // Poco::Environment::osVersion() returns version like "3.18.7-v7+"
      Poco::StringTokenizer tokenizer(Poco::Environment::osVersion(), ".-+");
      if (tokenizer.count() < 3)
         throw shared::exception::CException("Unable to get OS version");

      try
      {
         int major = boost::lexical_cast<int>(tokenizer[0]);
         int minor = boost::lexical_cast<int>(tokenizer[1]);
         int buildNumber = boost::lexical_cast<int>(tokenizer[2]);
         shared::versioning::CVersion version(major, minor, buildNumber);
         return version;         
      }
      catch(...)
      {
         throw shared::exception::CException("Unable to parse OS version");
      }  
   }
} //namespace tools
