#include "stdafx.h"
#include "YScriptApiImplementation.h"

namespace automation { namespace action { namespace script
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
   m_pluginGateway->sendCommandAsync(keywordId, newState);
}

void CYScriptApiImplementation::sendNotification(int keywordId, int recipientId, const std::string& message)
{
   //TODO
}

} } } // namespace automation::action::script
	
	