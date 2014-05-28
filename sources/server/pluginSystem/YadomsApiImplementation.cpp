#include "stdafx.h"
#include "YadomsApiImplementation.h"

namespace pluginSystem
{

CYadomsApiImplementation::CYadomsApiImplementation(int pluginInstanceId, boost::asio::io_service& pGlobalPluginIOService)
   :m_pluginInstanceId(pluginInstanceId), m_pGlobalPluginIOService(pGlobalPluginIOService)
{
}
      
CYadomsApiImplementation::~CYadomsApiImplementation() 
{
}

bool CYadomsApiImplementation::deviceExists(const std::string & deviceName)
{
/*
   using the database requester check for device existance
*/
   //TODO : !
   return false;
}

bool CYadomsApiImplementation::declareNewDevice(const std::string & deviceName, std::vector< shared::plugin::yadomsApi::CCapacity > & capacities)
{
/*
   using the database requester declare the device
*/

//TODO : !
   return false;
}
      
bool CYadomsApiImplementation::historizeData(const std::string & deviceName, const shared::plugin::yadomsApi::CCapacity & capacity, const std::string & value)
{
/*
   using the database requester save new capacity value
*/
      
   //TODO : !
   return false;
}
        
bool CYadomsApiImplementation::recordPluginEvent(PluginEventSeverity severity, const std::string & message)
{
/*
   using the PluginLogEvent record a new log entry
*/
      
   //TODO : !
   return false;
}
      
    
boost::asio::io_service & CYadomsApiImplementation::getPluginsIoService()
{
   return m_pGlobalPluginIOService;
}
	
} // namespace pluginSystem	
