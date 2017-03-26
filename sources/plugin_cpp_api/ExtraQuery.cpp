#include "stdafx.h"
#include "ExtraQuery.h"

namespace plugin_cpp_api
{
   CExtraQuery::CExtraQuery(const plugin_IPC::toPlugin::ExtraQuery& msg,
                            boost::function1<void, const shared::CDataContainer&> sucessCallback,
                            boost::function1<void, const std::string&> errorCallback,
                           boost::function2<void, const float, const std::string&> progressionCallback)
      : m_data(msg.query(), shared::CDataContainer(msg.data())),
        m_sucessCallback(sucessCallback),
        m_errorCallback(errorCallback),
        m_progressionCallback(progressionCallback)
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

   void CExtraQuery::reportProgress(const float progression, const std::string& message)
   {
      m_progressionCallback(progression, message);
   }
} // namespace plugin_cpp_api	


