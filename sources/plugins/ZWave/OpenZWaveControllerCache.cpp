#include "stdafx.h"
#include "OpenZWaveControllerCache.h"
#include <shared/Log.h>
#include "OpenZWaveHelpers.h"

COpenZWaveControllerCache::COpenZWaveControllerCache()
{
}


COpenZWaveControllerCache::~COpenZWaveControllerCache()
{
}

void COpenZWaveControllerCache::AddKeywordValue(const std::string & deviceName, boost::shared_ptr<CKeywordContainer> & info)
{
   m_keywordCache[deviceName].push_back(info);
}

void COpenZWaveControllerCache::AddDeviceInfo(const std::string & deviceName, shared::CDataContainer & info)
{
   m_deviceCache[deviceName].first.mergeFrom(info);
}

void COpenZWaveControllerCache::UpdateDeviceState(const std::string & deviceName, shared::plugin::yPluginApi::historization::EDeviceState & state)
{
   m_deviceCache[deviceName].second = state;
}

COpenZWaveControllerCache::DeviceCache & COpenZWaveControllerCache::getDeviceCache()
{
   return m_deviceCache;
}

COpenZWaveControllerCache::DeviceInfoAndState & COpenZWaveControllerCache::getDeviceInfo(const std::string & deviceName)
{
   return m_deviceCache[deviceName];
}

COpenZWaveControllerCache::KeywordList & COpenZWaveControllerCache::getKeywordsForDevice(const std::string & deviceName)
{
   return m_keywordCache[deviceName];
}

void COpenZWaveControllerCache::clearKeywordsForDevice(const std::string & deviceName)
{
   m_keywordCache[deviceName].clear();
}
