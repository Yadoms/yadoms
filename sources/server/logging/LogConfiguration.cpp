#include "stdafx.h"
#include "LogConfigurationImpl.h"
#include "LogConfiguration.h"

namespace logging
{
   //declare static PImpl configurator
   boost::shared_ptr<CLogConfigurationImpl> CLogConfiguration::m_configurator(boost::make_shared<CLogConfigurationImpl>());

   void CLogConfiguration::configure(const std::string& logLevel,
                                     const boost::filesystem::path& logPath)
   {
      m_configurator->configure(logLevel,
                                logPath);
   }
} // namespace logging


