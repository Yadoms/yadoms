#include "stdafx.h"
#include "InternalScriptApiImplementation.h"

namespace automation { namespace script
{

InternalScriptApiImplementation::InternalScriptApiImplementation(boost::shared_ptr<shared::script::ILogger> ruleLogger,
   boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
   boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
   boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
   boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
   boost::shared_ptr<database::IKeywordRequester> dbKeywordRequester,
   boost::shared_ptr<IGeneralInfo> generalInfo)
   : m_scriptApi(ruleLogger, pluginGateway, configurationManager, dbAcquisitionRequester, dbDeviceRequester, dbKeywordRequester, generalInfo)
{
}

InternalScriptApiImplementation::~InternalScriptApiImplementation()
{
}

shared::script::yScriptApi::IYScriptApi& InternalScriptApiImplementation::scriptApi()
{
   return m_scriptApi;
}

bool InternalScriptApiImplementation::ruleEnabled() const
{
   return m_scriptApi.ruleEnabled();
}

} } // namespace automation::script

