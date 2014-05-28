#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IYadomsApi implementation
   //-----------------------------------------------------
   class CYadomsApiImplementation : public shared::plugin::yadomsApi::IYadomsApi
   {
   public:
      //-----------------------------------------------------
      ///\brief Constructor
      //-----------------------------------------------------
      CYadomsApiImplementation(int pluginInstanceId, boost::asio::io_service& pGlobalPluginIOService);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApiImplementation();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string & deviceName);
      virtual bool declareNewDevice(const std::string & deviceName, std::vector< shared::plugin::yadomsApi::CCapacity > & capacities);
      virtual bool historizeData(const std::string & deviceName, const shared::plugin::yadomsApi::CCapacity & capacity, const std::string & value);
      virtual bool recordPluginEvent(PluginEventSeverity severity, const std::string & message);
      virtual boost::asio::io_service & getPluginsIoService();
      // [END] IYadomsApi implementation 
      
   private:
      int m_pluginInstanceId;
      boost::asio::io_service& m_pGlobalPluginIOService;
   };
	
} // namespace pluginSystem	
	
	