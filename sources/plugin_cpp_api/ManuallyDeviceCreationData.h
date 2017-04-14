#pragma once

#include <shared/plugin/yPluginApi/IManuallyDeviceCreationData.h>

namespace plugin_cpp_api
{
   class CManuallyDeviceCreationData : public shared::plugin::yPluginApi::IManuallyDeviceCreationData
   {
   public:
      CManuallyDeviceCreationData(const std::string& deviceName, 
                                  const std::string& configuration);
      virtual ~CManuallyDeviceCreationData();

      // IManuallyDeviceCreationData Implementation
      const std::string& getDeviceName() const override;
      const std::string& getDeviceType() const override;
      const shared::CDataContainer& getConfiguration() const override;
      // [END] IManuallyDeviceCreationData Implementation

   private:
      //-----------------------------------------------------
      ///\brief               Device name
      //-----------------------------------------------------
      const std::string m_deviceName;

      //-----------------------------------------------------
      ///\brief               Device type
      //-----------------------------------------------------
      std::string m_deviceType;

      //-----------------------------------------------------
      ///\brief               Device configuration
      //-----------------------------------------------------
      shared::CDataContainer m_configuration;
   };
} // namespace plugin_cpp_api	


