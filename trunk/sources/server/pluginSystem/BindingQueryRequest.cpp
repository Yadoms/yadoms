#include "stdafx.h"
#include "BindingQueryRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CBindingQueryRequest::CBindingQueryRequest(const shared::plugin::yadomsApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< shared::CDataContainer > & callback)
      :m_requestPtr(new communication::callback::CCallbackRequest<shared::plugin::yadomsApi::IBindingQueryData, shared::CDataContainer >(data, callback))
   {
   }
      
   CBindingQueryRequest::~CBindingQueryRequest()
   {
   }
   
   const shared::plugin::yadomsApi::IBindingQueryData& CBindingQueryRequest::getData()
   { 
      return m_requestPtr->getData(); 
   }

   void CBindingQueryRequest::sendSuccess(const shared::CDataContainer & newDeviceName)
   { 
      m_requestPtr->sendSuccess(newDeviceName); 
   }

   void CBindingQueryRequest::sendError(const std::string& errorMessage) 
   { 
      m_requestPtr->sendError(errorMessage); 
   }

} // namespace pluginSystem
	
	