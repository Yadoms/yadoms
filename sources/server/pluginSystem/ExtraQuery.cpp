#include "stdafx.h"
#include "ExtraQuery.h"
#include "communication/callback/CallbackRequest.h"

namespace pluginSystem
{
   CExtraQuery::CExtraQuery(const shared::plugin::yPluginApi::IExtraQueryData& data,
                            communication::callback::ISynchronousCallback<shared::CDataContainer>& callback)
      : m_requestPtr(boost::make_shared<communication::callback::CCallbackRequest<shared::plugin::yPluginApi::IExtraQueryData, shared::CDataContainer> >(data, callback))
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   const shared::plugin::yPluginApi::IExtraQueryData& CExtraQuery::getData() const
   {
      return m_requestPtr->getData();
   }

   void CExtraQuery::sendSuccess(const shared::CDataContainer& data)
   {
      m_requestPtr->sendSuccess(data);
   }

   void CExtraQuery::sendError(const std::string& errorMessage)
   {
      m_requestPtr->sendError(errorMessage);
   }
} // namespace pluginSystem	


