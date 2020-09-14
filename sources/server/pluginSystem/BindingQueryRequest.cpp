#include "stdafx.h"
#include "BindingQueryRequest.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CBindingQueryRequest::CBindingQueryRequest(const shared::plugin::yPluginApi::IBindingQueryData& data,
                                              communication::callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback)
      : m_requestPtr(boost::make_shared<communication::callback::CCallbackRequest<shared::plugin::yPluginApi::IBindingQueryData, boost::shared_ptr<shared::CDataContainer>>>(data, callback))
   {
   }

   CBindingQueryRequest::~CBindingQueryRequest()
   {
   }

   const shared::plugin::yPluginApi::IBindingQueryData& CBindingQueryRequest::getData()
   {
      return m_requestPtr->getData();
   }

   void CBindingQueryRequest::sendSuccess(const boost::shared_ptr<shared::CDataContainer>& newDeviceName)
   {
      m_requestPtr->sendSuccess(newDeviceName);
   }

   void CBindingQueryRequest::sendError(const std::string& errorMessage)
   {
      m_requestPtr->sendError(errorMessage);
   }
} // namespace pluginSystem


