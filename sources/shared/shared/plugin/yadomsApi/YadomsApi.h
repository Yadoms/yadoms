#pragma once
#include <shared/Export.h>
#include "IYadomsApi.h"


namespace shared { namespace plugin { namespace yadomsApi
{
   class CYadomsApi : public IYadomsApi
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      //-----------------------------------------------------
      CYadomsApi(int pluginInstanceId, boost::asio::io_service * pGlobalPluginIOService);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApi();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string & deviceName);
      virtual bool declareNewDevice(const std::string & deviceName, std::vector< CCapacity > & capacities);
      virtual bool historizeData(const std::string & deviceName, const CCapacity & capacity, const std::string & value);
      virtual bool recordPluginEvent(PluginEventSeverity severity, const std::string & message);
      virtual boost::asio::io_service & getPluginsIoService();
      // [END] IYadomsApi implementation 
      
   private:
      int m_pluginInstanceId;
      boost::asio::io_service * m_pGlobalPluginIOService;
   };
	
} } } // namespace shared::plugin::yadomsApi	
	
	