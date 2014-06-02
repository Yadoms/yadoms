#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include "database/entities/Entities.h"


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
      //TODO commenter
      //-----------------------------------------------------
      CYadomsApiImplementation(const boost::shared_ptr<database::entities::CPlugin> pluginData, boost::asio::io_service& pGlobalPluginIOService);
      
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~CYadomsApiImplementation();

      // IYadomsApi implementation 
      virtual bool deviceExists(const std::string & deviceName);
      virtual bool declareNewDevice(const std::string & deviceName, const std::vector<shared::plugin::yadomsApi::CCapacity> & capacities);
      virtual void historizeData(const std::string & deviceName, const shared::plugin::yadomsApi::CCapacity & capacity, const std::string & value);
      virtual const std::string getConfiguration() const;
      virtual bool recordPluginEvent(PluginEventSeverity severity, const std::string & message);
      virtual boost::asio::io_service & getPluginsIoService();
      // [END] IYadomsApi implementation 
      
      //-----------------------------------------------------
      ///\brief Get instance ID
      //-----------------------------------------------------
      virtual int getInstanceId() const;

   private:
      //--------------------------------------------------------------
      /// \brief			Plugin IOService (common for all plugin instances)
      //--------------------------------------------------------------
      boost::asio::io_service& m_pGlobalPluginIOService;

      //--------------------------------------------------------------
      /// \brief			The database accessor
      //--------------------------------------------------------------
      const boost::shared_ptr<database::entities::CPlugin> m_pluginData;
   };
	
} // namespace pluginSystem	
	
	