#include "stdafx.h"
#include "SomfySituoConfiguration.h"
#include <shared/Log.h>

CSomfySituoConfiguration::CSomfySituoConfiguration()
   :m_data(new_CDataContainerSharedPtr())
{
}

CSomfySituoConfiguration::~CSomfySituoConfiguration()
{
}

void CSomfySituoConfiguration::initializeWith(const shared::CDataContainerSharedPtr& data)
{
   m_data->initializeWith(data);
}

std::string CSomfySituoConfiguration::getSerialPort() const
{
   return m_data->get<std::string>("SerialPort");
}
