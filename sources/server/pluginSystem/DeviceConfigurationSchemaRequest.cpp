#include "stdafx.h"
#include "DeviceConfigurationSchemaRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CDeviceConfigurationSchemaRequest::CDeviceConfigurationSchemaRequest(const std::string& device,
                                                                        communication::callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
      : m_requestPtr(boost::make_shared<communication::callback::CCallbackRequest<const std::string&, boost::shared_ptr<shared::CDataContainer>> >(device, callback))
   {
   }

   CDeviceConfigurationSchemaRequest::~CDeviceConfigurationSchemaRequest()
   {
   }

   std::string CDeviceConfigurationSchemaRequest::device() const
   {
      return m_requestPtr->getData();
   }

   void CDeviceConfigurationSchemaRequest::sendSuccess(const boost::shared_ptr<shared::CDataContainer>& newDeviceName)
   {
      m_requestPtr->sendSuccess(newDeviceName);
   }

   void CDeviceConfigurationSchemaRequest::sendError(const std::string& errorMessage)
   {
      m_requestPtr->sendError(errorMessage);
   }
} // namespace pluginSystem


