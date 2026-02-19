#include "stdafx.h"
#include "ExtraQuery.h"

#include <utility>
#include "ExtraQueryData.h"

namespace plugin_cpp_api
{
   CExtraQuery::CExtraQuery(const plugin_IPC::toPlugin::ExtraQuery& msg,
                            boost::function1<void, const boost::shared_ptr<shared::CDataContainer>&> sucessCallback,
                            boost::function1<void, const std::string&> errorCallback,
                            boost::function2<void, float, const std::string&> progressionCallback)
      : m_data(boost::make_shared<CExtraQueryData>(msg.query(),
                                                   shared::CDataContainer::make(msg.data()),
                                                   msg.device())),
        m_sucessCallback(std::move(sucessCallback)),
        m_errorCallback(std::move(errorCallback)),
        m_progressionCallback(std::move(progressionCallback))
   {
   }

   boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> CExtraQuery::getData() const
   {
      return m_data;
   }

   void CExtraQuery::sendSuccess(const boost::shared_ptr<shared::CDataContainer>& result)
   {
      m_sucessCallback(result);
   }

   void CExtraQuery::sendError(const std::string& errorMessage)
   {
      m_errorCallback(errorMessage);
   }

   void CExtraQuery::reportProgress(float progression,
                                    const std::string& message)
   {
      m_progressionCallback(progression, message);
   }
} // namespace plugin_cpp_api	
