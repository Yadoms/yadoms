#include "stdafx.h"
#include "LogConfigurationImpl.h"
#include "LogConfiguration.h"

namespace logging
{

	void CLogConfiguration::configure(const std::string& logLevel,
		                              const boost::filesystem::path& logPath)
	{
		if (!m_configurator)
			m_configurator = boost::make_shared<CLogConfigurationImpl>();

		m_configurator->configure(logLevel,
			                      logPath);
	}
} // namespace logging


