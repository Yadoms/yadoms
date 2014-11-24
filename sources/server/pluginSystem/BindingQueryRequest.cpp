#include "stdafx.h"
#include "BindingQueryRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CBindingQueryRequest::CBindingQueryRequest(const shared::plugin::yadomsApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< std::vector<std::string> > & callback)
      :m_requestPtr(new communication::callback::CCallbackRequest<shared::plugin::yadomsApi::IBindingQueryData, std::vector<std::string> >(data, callback))
   {
   }
      
   CBindingQueryRequest::~CBindingQueryRequest()
   {
   }
   
   const shared::plugin::yadomsApi::IBindingQueryData& CBindingQueryRequest::getData()
   { 
      return m_requestPtr->getData(); 
   }

   void CBindingQueryRequest::sendSuccess(const std::vector<std::string> & newDeviceName) 
   { 
      m_requestPtr->sendSuccess(newDeviceName); 
   }

   void CBindingQueryRequest::sendError(const std::string& errorMessage) 
   { 
      m_requestPtr->sendError(errorMessage); 
   }

} // namespace pluginSystem
	
	