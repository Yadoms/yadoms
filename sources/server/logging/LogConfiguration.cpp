#include "stdafx.h"
#include "LogConfigurationImpl.h"
#include "LogConfiguration.h"

namespace logging
{
   //declare static PImpl configurator
   boost::shared_ptr<CLogConfigurationImpl> CLogConfiguration::m_configurator(new CLogConfigurationImpl());
   
   void CLogConfiguration::configure(const std::string & logLevel)
   {
      m_configurator->configure(logLevel);
   }

} // namespace logging