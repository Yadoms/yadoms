#pragma once
#include "LogConfigurationImpl.h"

namespace logging
{
   //--------------------------------------------------------------
   /// \brief     Class used to configure the logger
   //--------------------------------------------------------------
   class CLogConfiguration final
   {
   public:
      explicit CLogConfiguration(const std::string& logLevel,
                                 const boost::filesystem::path& logPath);
      ~CLogConfiguration() = default;

   private:
      boost::shared_ptr<CLogConfigurationImpl> m_configurator;
   };
} // namespace logging
