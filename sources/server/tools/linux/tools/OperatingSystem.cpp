#include "stdafx.h"
#include "OperatingSystem.h"
#include <Poco/Environment.h>
#include <Poco/StringTokenizer.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>

namespace tools
{
   bool COperatingSystem::shutdown(bool andRestart)
   {
      sync();
      auto rc = system(andRestart ? "shutdown --reboot now" : "shutdown --poweroff now");
      if (rc != 0)
      {
         auto errorMsg = strerror(rc);
         YADOMS_LOG(error) << "Error when try to shutdown/reboot : " << errorMsg;
         return false;
      }
      return true;
   }

   std::string COperatingSystem::getName()
   {
      return "linux";
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

   Poco::ProcessHandle COperatingSystem::launchNativeScript(const std::string& scriptPath,
                                                            const Poco::Process::Args& args)
   {
      args.insert(args.begin(), scriptPath);

      YADOMS_LOG(debug) << "launchNativeScript sh with args " << boost::algorithm::join(args, ", ");

      return Poco::Process::launch("sh", args);
   }
   
} //namespace tools
