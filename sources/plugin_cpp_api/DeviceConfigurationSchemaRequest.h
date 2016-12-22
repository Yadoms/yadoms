#pragma once
#include <toPlugin.pb.h>
#include <shared/plugin/yPluginApi/IDeviceConfigurationSchemaRequest.h>

namespace plugin_cpp_api
{
   class CDeviceConfigurationSchemaRequest : public shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest
   {
   public:
      CDeviceConfigurationSchemaRequest(const plugin_IPC::toPlugin::DeviceConfigurationSchemaRequest& msg,
                                        boost::function1<void, const shared::CDataContainer&> sucessCallback,
                                        boost::function1<void, const std::string&> errorCallback);
      virtual ~CDeviceConfigurationSchemaRequest();

      // IDeviceConfigurationSchemaRequest Implementation
      std::string device() const override;
      void sendSuccess(const shared::CDataContainer& deviceConfigurationSchema) override;
      void sendError(const std::string& errorMessage) override;
      // [END] IDeviceConfigurationSchemaRequest Implementation

   private:
      const std::string m_device;
      boost::function1<void, const shared::CDataContainer&> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace plugin_cpp_api	


