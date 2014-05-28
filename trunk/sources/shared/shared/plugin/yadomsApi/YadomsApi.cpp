#include "stdafx.h"
#include "IYadomsApi.h"

//should not be here, this implementation should remain in server::pluginSystem
namespace shared { namespace plugin { namespace yadomsApi
{

      CYadomsApi::CYadomsApi(int pluginInstanceId, boost::asio::io_service * pGlobalPluginIOService)
         :m_pluginInstanceId(pluginInstanceId), m_pGlobalPluginIOService(pGlobalPluginIOService)
      {
      }
      
      CYadomsApi::~CYadomsApi() 
      {
      }

      bool CYadomsApi::deviceExists(const std::string & deviceName)
      {
      /*
         using the database requester check for device existance
      */
         //TODO : !
         return false;
      }

      bool CYadomsApi::declareNewDevice(const std::string & deviceName, std::vector< CCapacity > & capacities)
      {
      /*
         using the database requester declare the device
      */

      //TODO : !
         return false;
      }
      
      bool CYadomsApi::historizeData(const std::string & deviceName, const CCapacity & capacity, const std::string & value)
      {
      /*
         using the database requester save new capacity value
      */
      
         //TODO : !
         return false;
      }
        
      bool CYadomsApi::recordPluginEvent(PluginEventSeverity severity, const std::string & message)
      {
      /*
         using the PluginLogEvent record a new log entry
      */
      
         //TODO : !
         return false;
      }
      
    
      boost::asio::io_service & CYadomsApi::getPluginsIoService()
      {
         return m_pGlobalPluginIOService;
      }
   };
	
} } } // namespace shared::plugin::yadomsApi	
	
	