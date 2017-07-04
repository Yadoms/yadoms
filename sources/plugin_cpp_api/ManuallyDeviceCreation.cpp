#include "stdafx.h"
#include "ManuallyDeviceCreation.h"


namespace plugin_cpp_api
{
   CManuallyDeviceCreation::CManuallyDeviceCreation(const plugin_IPC::toPlugin::ManuallyDeviceCreation& msg,
                                                    boost::function0<void> sucessCallback,
                                                    boost::function1<void, const std::string&> errorCallback)
      : m_data(msg.name(), msg.type(), msg.configuration()),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CManuallyDeviceCreation::~CManuallyDeviceCreation()
   {
   }

   const shared::plugin::yPluginApi::IManuallyDeviceCreationData& CManuallyDeviceCreation::getData() const
   {
      return m_data;
   }

   void CManuallyDeviceCreation::sendSuccess()
   {
      m_sucessCallback();
   }

   void CManuallyDeviceCreation::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace plugin_cpp_api	


