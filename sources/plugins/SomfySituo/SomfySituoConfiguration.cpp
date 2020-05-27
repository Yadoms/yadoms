#include "stdafx.h"
#include "SomfySituoConfiguration.h"
#include <shared/Log.h>

CSomfySituoConfiguration::CSomfySituoConfiguration()
   :m_data(shared::CDataContainer::make())
{
}

CSomfySituoConfiguration::~CSomfySituoConfiguration()
{
}

void CSomfySituoConfiguration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data->initializeWith(data);
}

std::string CSomfySituoConfiguration::getSerialPort() const
{
   return m_data->get<std::string>("SerialPort");
}
