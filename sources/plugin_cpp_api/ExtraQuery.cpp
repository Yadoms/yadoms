#include "stdafx.h"
#include "ExtraQuery.h"

namespace plugin_cpp_api
{
   CExtraQuery::CExtraQuery(const toPlugin::ExtraQuery& msg,
                            boost::function1<void, const shared::CDataContainer&> sucessCallback,
                            boost::function1<void, const std::string&> errorCallback)
      : m_data(msg.query(), shared::CDataContainer(msg.data())),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback)
   {
   }

   CExtraQuery::~CExtraQuery()
   {
   }

   const shared::plugin::yPluginApi::IExtraQueryData& CExtraQuery::getData() const
   {
      return m_data;
   }

   void CExtraQuery::sendSuccess(const shared::CDataContainer& result)
   {
      m_sucessCallback(result);
   }

   void CExtraQuery::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }
} // namespace plugin_cpp_api	


