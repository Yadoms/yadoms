#pragma once
#include "LogConfigurationImpl.h"

namespace logging
{
   //--------------------------------------------------------------
   /// \brief     Class used to configure the logger
   //--------------------------------------------------------------
   class CLogConfiguration
   {
   public:

      //--------------------------------------------------------------
      /// \brief	                  configure the logger with one file per thread without rolling on size
      /// \param[in]  logLevel      The log level
      /// \param[in]  logPath       The log path
      //--------------------------------------------------------------
      static void configure(const std::string& logLevel,
                            const boost::filesystem::path& logPath);

   private:
      //--------------------------------------------------------------
      /// \brief     Private impelmentation
      //--------------------------------------------------------------
      static boost::shared_ptr<CLogConfigurationImpl> m_configurator;
   };
} // namespace logging


