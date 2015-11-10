#pragma once
#include <shared/versioning/Version.h>

namespace tools {

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
   };

} //namespace tools