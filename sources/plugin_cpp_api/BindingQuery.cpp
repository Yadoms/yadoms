#include "stdafx.h"
#include "BindingQuery.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>


CBindingQuery::CBindingQuery(const toPlugin::BindingQuery& msg,
   shared::event::CEventHandler& pluginEventHandler,
   boost::function1<void, const shared::CDataContainer&> sucessCallback,
   boost::function1<void, const std::string&> errorCallback)
   :m_query(msg.query()),
   m_pluginEventHandler(pluginEventHandler),
   m_sucessCallback(sucessCallback),
   m_errorCallback(errorCallback)
{
}

CBindingQuery::~CBindingQuery()
{
}

const shared::plugin::yPluginApi::IBindingQueryData& CBindingQuery::getData()
{
   return m_query;
}

void CBindingQuery::sendSuccess(const shared::CDataContainer& result)
{
   m_sucessCallback(result);
}

void CBindingQuery::sendError(const std::string& errorMessage)
{
   m_errorCallback(errorMessage);
}

void CBindingQuery::askToPlugin() const//TODO virer ? (avec m_pluginEventHandler)
{
   m_pluginEventHandler.postEvent(shared::plugin::yPluginApi::IYPluginApi::kBindingQuery, m_query);
}
