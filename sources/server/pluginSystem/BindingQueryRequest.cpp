#include "stdafx.h"
#include "BindingQueryRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CBindingQueryRequest::CBindingQueryRequest(const shared::plugin::yPluginApi::IBindingQueryData& data,
                                              communication::callback::ISynchronousCallback<shared::CDataContainerSharedPtr>& callback)
      : m_requestPtr(boost::make_shared<communication::callback::CCallbackRequest<shared::plugin::yPluginApi::IBindingQueryData, shared::CDataContainerSharedPtr>>(data, callback))
   {
   }

   CBindingQueryRequest::~CBindingQueryRequest()
   {
   }

   const shared::plugin::yPluginApi::IBindingQueryData& CBindingQueryRequest::getData()
   {
      return m_requestPtr->getData();
   }

   void CBindingQueryRequest::sendSuccess(const shared::CDataContainerSharedPtr& newDeviceName)
   {
      m_requestPtr->sendSuccess(newDeviceName);
   }

   void CBindingQueryRequest::sendError(const std::string& errorMessage)
   {
      m_requestPtr->sendError(errorMessage);
   }
} // namespace pluginSystem


