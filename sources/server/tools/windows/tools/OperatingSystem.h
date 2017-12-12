#pragma once
#include <shared/versioning/Version.h>
#include <Poco/Process.h>

namespace tools
{
   class COperatingSystem
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                  Shutdown the system
      /// \param[in] andRestart     Shutdown and restart if true
      /// \return                   true if success
      //--------------------------------------------------------------
      static bool shutdown(bool andRestart = false);

      //--------------------------------------------------------------
      /// \brief	                  Get the system name
      /// \return                   System name
      //--------------------------------------------------------------
      static std::string getName();

      //--------------------------------------------------------------
      /// \brief	                  Get the system version (converted to CVersion)
      /// \return                   System version
      /// \throw shared::exception::CException if error getting OS version
      //--------------------------------------------------------------
      static shared::versioning::CVersion getVersion();

      //--------------------------------------------------------------
      /// \brief	                  Launch native script (batch for Windows, bash for Linux)
      /// \param[in] scriptPath     The script path (dir + filename + extension)
      /// \param[in] args           The script args
      /// \return                   The started process handle
      //--------------------------------------------------------------
      static Poco::ProcessHandle COperatingSystem::launchNativeScript(const std::string& scriptPath,
                                                                      const Poco::Process::Args& args);
   };
} //namespace tools
