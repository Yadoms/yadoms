#pragma once
#include <plugin_IPC/yadomsToPlugin.pb.h>
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "ManuallyDeviceCreationData.h"

namespace plugin_cpp_api
{
   class CManuallyDeviceCreation : public shared::plugin::yPluginApi::IManuallyDeviceCreationRequest
   {
   public:
      CManuallyDeviceCreation(const plugin_IPC::toPlugin::ManuallyDeviceCreation& msg,
                              boost::function0<void> sucessCallback,
                              boost::function1<void, const std::string&> errorCallback);
      virtual ~CManuallyDeviceCreation();

      // IManuallyDeviceCreationRequest Implementation
      const shared::plugin::yPluginApi::IManuallyDeviceCreationData& getData() const override;
      void sendSuccess() override;
      void sendError(const std::string& errorMessage) override;
      // [END] IManuallyDeviceCreationRequest Implementation

   private:
      CManuallyDeviceCreationData m_data;
      boost::function0<void> m_sucessCallback;
      boost::function1<void, const std::string&> m_errorCallback;
   };
} // namespace plugin_cpp_api	


