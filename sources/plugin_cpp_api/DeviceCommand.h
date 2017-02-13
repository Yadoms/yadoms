#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>


namespace plugin_cpp_api
{
   class CDeviceCommand : public shared::plugin::yPluginApi::IDeviceCommand
   {
   public:
      explicit CDeviceCommand(const plugin_IPC::toPlugin::DeviceCommand& msg);
      virtual ~CDeviceCommand();

      // IDeviceCommand Implementation
      const std::string& getDevice() const override;
      const std::string& getKeyword() const override;
      const std::string& getBody() const override;
      // [END] IDeviceCommand Implementation

   private:

      const std::string m_device;
      const std::string m_keyword;
      const std::string m_body;
   };
} // namespace plugin_cpp_api	
