#pragma once
#include "IPluginStateHelper.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	The default pairing helper implementation (based on manual/auto mode)
//--------------------------------------------------------------
class CPluginStateHelper : public IPluginStateHelper
{
public:
   explicit CPluginStateHelper(boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CPluginStateHelper();

   // IPluginStateHelper implementation
   void set(EState internalState,
            const std::string& customMessageId = std::string(),
            const std::map<std::string, std::string>& customMessageDataParams = std::map<std::string, std::string>()) override;
   // [END] IPluginStateHelper implementation

protected:
   void notifyServer(EState internalState,
                     const std::string& customMessageId = std::string(),
                     const std::map<std::string, std::string>& customMessageDataParams = std::map<std::string, std::string>()) const;

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   EState m_lastInternalState;
   std::string m_lastMessage;
   std::map<std::string, std::string> m_lastMessageData;
};
