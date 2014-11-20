#include "stdafx.h"
#include "CustomQueryListRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CCustomQueryListRequest::CCustomQueryListRequest(const shared::plugin::yadomsApi::ICustomQueryData & data, communication::callback::ISynchronousCallback< std::vector<std::string> > & callback)
      :m_requestPtr(new communication::callback::CCallbackRequest<shared::plugin::yadomsApi::ICustomQueryData, std::vector<std::string> >(data, callback))
   {
   }
      
   CCustomQueryListRequest::~CCustomQueryListRequest()
   {
   }
   
   const shared::plugin::yadomsApi::ICustomQueryData& CCustomQueryListRequest::getData()
   { 
      return m_requestPtr->getData(); 
   }

   void CCustomQueryListRequest::sendSuccess(const std::vector<std::string> & newDeviceName) 
   { 
      m_requestPtr->sendSuccess(newDeviceName); 
   }

   void CCustomQueryListRequest::sendError(const std::string& errorMessage) 
   { 
      m_requestPtr->sendError(errorMessage); 
   }

} // namespace pluginSystem
	
	