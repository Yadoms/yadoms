#include "stdafx.h"
#include "LogConfigurationImpl.h"
#include "LogConfiguration.h"

namespace logging
{
   CLogConfiguration::CLogConfiguration(const std::string& logLevel,
                                        const boost::filesystem::path& logPath)
      : m_configurator(boost::make_shared<CLogConfigurationImpl>())
   {
      m_configurator->configure(logLevel,
                                logPath);
   }
} // namespace logging
