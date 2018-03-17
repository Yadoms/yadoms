#include "stdafx.h"
#include "SomfyIOControllerConfiguration.h"
#include <shared/Log.h>

CSomfyIOControllerConfiguration::~CSomfyIOControllerConfiguration()
{
}

void CSomfyIOControllerConfiguration::initializeWith(const shared::CDataContainer& data)
{
	m_data.initializeWith(data);
}

std::string CSomfyIOControllerConfiguration::getSerialPort() const
{
	return m_data.get<std::string>("SerialPort");
}

