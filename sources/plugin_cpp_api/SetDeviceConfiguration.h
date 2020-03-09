#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/ISetDeviceConfiguration.h>


namespace plugin_cpp_api
{
   class CSetDeviceConfiguration : public shared::plugin::yPluginApi::ISetDeviceConfiguration
   {
   public:
      explicit CSetDeviceConfiguration(const plugin_IPC::toPlugin::SetDeviceConfiguration& msg);
      virtual ~CSetDeviceConfiguration();

      // ISetDeviceConfiguration Implementation
      const std::string& name() const override;
      const shared::CDataContainerSharedPtr& configuration() const override;
      // [END] ISetDeviceConfiguration Implementation

   private:
      const std::string m_device;
      const shared::CDataContainerSharedPtr m_configuration;
   };
} // namespace plugin_cpp_api	
