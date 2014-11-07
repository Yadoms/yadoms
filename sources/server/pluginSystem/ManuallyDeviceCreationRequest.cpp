#include "stdafx.h"
#include "ManuallyDeviceCreationRequest.h"
#include "communication\callback\CallbackRequest.h"

namespace pluginSystem
{
   CManuallyDeviceCreationRequest::CManuallyDeviceCreationRequest(const shared::plugin::yadomsApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback)
      :m_requestPtr(new communication::callback::CCallbackRequest<shared::plugin::yadomsApi::IManuallyDeviceCreationData, std::string>(data, callback))
   {
   }
      
   CManuallyDeviceCreationRequest::~CManuallyDeviceCreationRequest()
   {
   }
   
   const shared::plugin::yadomsApi::IManuallyDeviceCreationData& CManuallyDeviceCreationRequest::getData()
   { 
      return m_requestPtr->getData(); 
   }

   void CManuallyDeviceCreationRequest::sendSuccess(const std::string& newDeviceName) 
   { 
      m_requestPtr->sendSuccess(newDeviceName); 
   }

   void CManuallyDeviceCreationRequest::sendError(const std::string& errorMessage) 
   { 
      m_requestPtr->sendError(errorMessage); 
   }

} // namespace pluginSystem
	
	