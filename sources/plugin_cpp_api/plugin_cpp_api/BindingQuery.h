#pragma once

#include <toPlugin.pb.h>
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include "BindingQueryData.h"

class CBindingQuery : public shared::plugin::yPluginApi::IBindingQueryRequest
{
public:
   CBindingQuery(const toPlugin::BindingQuery& msg,
                 shared::event::CEventHandler& pluginEventHandler,
                 boost::function1<void, const shared::CDataContainer&> sucessCallback,
                 boost::function1<void, const std::string&> errorCallback);
   virtual ~CBindingQuery();

   // IBindingQueryRequest Implementation
   const shared::plugin::yPluginApi::IBindingQueryData& getData() override;
   void sendSuccess(const shared::CDataContainer& result) override;
   void sendError(const std::string& errorMessage) override;
   // [END] IBindingQueryRequest Implementation

   void askToPlugin() const;

private:
   CBindingQueryData m_query;
   shared::event::CEventHandler& m_pluginEventHandler;
   boost::function1<void, const shared::CDataContainer&> m_sucessCallback;
   boost::function1<void, const std::string&> m_errorCallback;
};
