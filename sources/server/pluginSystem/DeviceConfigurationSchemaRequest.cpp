#include "stdafx.h"
#include "DeviceConfigurationSchemaRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CDeviceConfigurationSchemaRequest::CDeviceConfigurationSchemaRequest(const std::string& device,
                                                                        communication::callback::ISynchronousCallback<shared::CDataContainer>& callback)
      : m_requestPtr(boost::make_shared<communication::callback::CCallbackRequest<const std::string&, shared::CDataContainer> >(device, callback))
   {
   }

   CDeviceConfigurationSchemaRequest::~CDeviceConfigurationSchemaRequest()
   {
   }

   std::string CDeviceConfigurationSchemaRequest::device() const
   {
      return m_requestPtr->getData();
   }

   void CDeviceConfigurationSchemaRequest::sendSuccess(const shared::CDataContainer& newDeviceName)
   {
      m_requestPtr->sendSuccess(newDeviceName);
   }

   void CDeviceConfigurationSchemaRequest::sendError(const std::string& errorMessage)
   {
      m_requestPtr->sendError(errorMessage);
   }
} // namespace pluginSystem


