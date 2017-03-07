#include "stdafx.h"
#include "DeviceConfigurationSchemaRequest.h"


namespace plugin_cpp_api
{
   CDeviceConfigurationSchemaRequest::CDeviceConfigurationSchemaRequest(const plugin_IPC::toPlugin::DeviceConfigurationSchemaRequest& msg,
                                                                        boost::function1<void, const shared::CDataContainer&> sucessCallback,
                                                                        boost::function1<void, const std::string&> errorCallback)
      : m_device(msg.device()),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CDeviceConfigurationSchemaRequest::~CDeviceConfigurationSchemaRequest()
   {
   }

   std::string CDeviceConfigurationSchemaRequest::device() const
   {
      return m_device;
   }

   void CDeviceConfigurationSchemaRequest::sendSuccess(const shared::CDataContainer& deviceConfigurationSchema)
   {
      m_sucessCallback(deviceConfigurationSchema);
   }

   void CDeviceConfigurationSchemaRequest::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace plugin_cpp_api	


