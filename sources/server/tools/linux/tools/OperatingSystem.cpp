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
      if (rc == -1 || WEXITSTATUS(rc) != 0)
      {
         YADOMS_LOG(warning) << "Error when try to shutdown/reboot : " << strerror(rc) << ", try with sudo...";

         rc = system(andRestart ? "sudo shutdown --reboot now" : "sudo shutdown --poweroff now");
         if (rc == -1 || WEXITSTATUS(rc) != 0)
         {
            YADOMS_LOG(error) << "Error when try to shutdown/reboot (with sudo) : " << strerror(rc);
            return false;
         }

         return true;
      }
      return true;
   }

   std::string COperatingSystem::getName()
   {
      return "linux";
   }
   
   shared::versioning::CSemVer COperatingSystem::getVersion()
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
         shared::versioning::CSemVer version(major, minor, buildNumber);
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
      YADOMS_LOG(debug) << "launchNativeScript : convert script EOL if needed...";

      // Script can have Windows EOL, so adapt to Unix EOL
      std::ifstream infile(scriptPath);
      std::stringstream buffer;
      buffer << infile.rdbuf();
      auto content = buffer.str();
      boost::replace_all(content, "\r\n", "\n");
      const auto normalizedScriptPath(boost::filesystem::path(scriptPath).parent_path() / (std::string("normalized_") + boost::filesystem::path(scriptPath).filename().string()));
      std::ofstream outfile(normalizedScriptPath.string(), std::ios::out | std::ios::trunc);
      outfile << content;
      outfile.close();

      // Make a full copy of args list to insert script path
      Poco::Process::Args nativeArgs;
      nativeArgs.push_back(normalizedScriptPath.string());
      for (const auto& arg : args)
         nativeArgs.push_back(arg);

      YADOMS_LOG(information) << "launchNativeScript : start update script :";
      YADOMS_LOG(information) << "   sh " << boost::algorithm::join(nativeArgs, " ");
      return Poco::Process::launch("sh", nativeArgs);
   }
   
} //namespace tools
