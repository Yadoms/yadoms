#include "stdafx.h"
#include "SomfySituoConfiguration.h"
#include <shared/Log.h>

CSomfySituoConfiguration::~CSomfySituoConfiguration()
{
}

void CSomfySituoConfiguration::initializeWith(const shared::CDataContainer& data)
{
	m_data.initializeWith(data);
}

std::string CSomfySituoConfiguration::getSerialPort() const
{
	return m_data.get<std::string>("SerialPort");
}
