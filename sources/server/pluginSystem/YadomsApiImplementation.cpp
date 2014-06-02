#include "stdafx.h"
#include "YadomsApiImplementation.h"

namespace pluginSystem
{

CYadomsApiImplementation::CYadomsApiImplementation(const boost::shared_ptr<database::entities::CPlugin> pluginData, boost::asio::io_service& pGlobalPluginIOService)
   :m_pGlobalPluginIOService(pGlobalPluginIOService), m_pluginData(pluginData)
{
}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string & deviceName)
{
/*
   using the database requester check for device existence
*/
   //TODO : !
   return false;
}

bool CYadomsApiImplementation::declareNewDevice(const std::string & deviceName, const std::vector<shared::plugin::yadomsApi::CCapacity> & capacities)
{
/*
   using the database requester declare the device
*/

//TODO : !
   return false;
}
      
void CYadomsApiImplementation::historizeData(const std::string & deviceName, const std::string & keyword, const shared::plugin::yadomsApi::CCapacity & capacity, const std::string & value)
{
/*
   using the database requester save new capacity value
*/
      
   //TODO : !
}

const std::string CYadomsApiImplementation::getConfiguration() const
{
   return m_pluginData->Configuration;
}
        
bool CYadomsApiImplementation::recordPluginEvent(PluginEventSeverity severity, const std::string & message)
{
/*
   using the PluginLogEvent record a new log entry
*/
      
   //TODO : !
   return false;
}
      
    
boost::asio::io_service & CYadomsApiImplementation::getPluginsIoService() const
{
   return m_pGlobalPluginIOService;
}

shared::event::CEventHandler & CYadomsApiImplementation::getEventHandler()
{
   return m_pluginEventHandler;
}

int CYadomsApiImplementation::getInstanceId() const
{
   return m_pluginData->Id;
}
	
} // namespace pluginSystem	
