#include "stdafx.h"
#include "MqttConfiguration.h"
#include <shared/Log.h>

CMqttConfiguration::~CMqttConfiguration()
{
}

void CMqttConfiguration::initializeWith(const shared::CDataContainer& data)
{
   m_configuration.initializeWith(data);
}

bool CMqttConfiguration::getStartServer() const
{
   return m_configuration.get<bool>("server.checked");
}

int CMqttConfiguration::getServerPort() const
{
   return m_configuration.get<int>("server.content.serverPort");
}

void CMqttConfiguration::trace() const
{
   try
   {
      YADOMS_LOG(information) << "CMqtt configuration : ";
      
   }
   catch (const shared::exception::CInvalidParameter& e)
   {
      YADOMS_LOG(error) << "Parameter not found : " << e.what();
   }
   catch (const shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Parameter value out of range : " << e.what();
   }
}