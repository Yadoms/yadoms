#include "stdafx.h"
#include "../../plugins/TeleInfo/Decoder.h"
#include <shared/Log.h>

CDecoder::CDecoder(boost::shared_ptr<yApi::IYPluginApi> api)
{}

void CDecoder::decodeTeleInfoMessage(boost::shared_ptr<yApi::IYPluginApi> api,
                                     const boost::shared_ptr<std::map<std::string, std::string>>& messages)
{}

void CDecoder::createDeviceAndKeywords(const bool monoPhase)
{}

void CDecoder::constructKeywordList(const EContracts contract, const bool monoPhase)
{}

void CDecoder::declareKeywordList(const std::string& tariff)
{}

bool CDecoder::isERDFCounterDesactivated() const
{
   return true;
}

void CDecoder::processMessage(const std::string& key,
                              const std::string& value)
{}