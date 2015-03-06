#include "stdafx.h"
#include "YScriptApiImplementation.h"
#include "../pluginSystem/DeviceCommand.h"
#include <shared/plugin/yPluginApi/historization/MessageFormatter.h>
#include <shared/Log.h>
#include <shared/exception/EmptyResult.hpp>

namespace automation { namespace script
{

CYScriptApiImplementation::CYScriptApiImplementation(
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter,
   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_pluginGateway(pluginGateway),
   m_notificationCenter(notificationCenter),
   m_dbAcquisitionRequester(dbAcquisitionRequester)
{
}

CYScriptApiImplementation::~CYScriptApiImplementation()
{
}

std::string CYScriptApiImplementation::readKeyword(int keywordId) const
{
   return m_dbAcquisitionRequester->getKeywordLastData(keywordId)->Value;
}

std::string CYScriptApiImplementation::waitForEvent(int keywordId, const std::string& timeout) const
{
   //TODO
   return std::string();
}

std::pair<int, std::string> CYScriptApiImplementation::waitForEvents(std::vector<int> keywordIdList, const std::string& timeout) const
{
   //TODO
   return std::pair<int, std::string>();
}

void CYScriptApiImplementation::writeKeyword(int keywordId, const std::string& newState)
{
   try
   {
      m_pluginGateway->sendCommandAsync(keywordId, newState);
   }
   catch (shared::exception::CEmptyResult& e)
   {
      YADOMS_LOG(error) << "Unable to write keyword : " << e.what();
   }
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   shared::plugin::yPluginApi::historization::CMessageFormatter body(0, recipientId, message);
   m_pluginGateway->sendCommandAsync(keywordId, body.formatValue());
}

} } // namespace automation::script
	
	