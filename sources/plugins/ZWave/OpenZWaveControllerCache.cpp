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
	if (m_deviceCache.find(deviceName) == m_deviceCache.end())
	{
		m_deviceCache.insert(std::make_pair(deviceName, CDeviceInfoAndState(info, shared::plugin::yPluginApi::historization::EDeviceState::kUnknown)));
	}
	else
	{
		//m_deviceCache[deviceName].getDeviceInfo().mergeFrom(info);
		m_deviceCache[deviceName].setDeviceInfo(info);
	}
}

void COpenZWaveControllerCache::UpdateDeviceState(const std::string & deviceName, shared::plugin::yPluginApi::historization::EDeviceState & state)
{
	if (m_deviceCache.find(deviceName) == m_deviceCache.end())
	{
		m_deviceCache.insert(std::make_pair(deviceName, CDeviceInfoAndState(shared::CDataContainer(), state)));
	}
	else
	{
		m_deviceCache[deviceName].setDeviceState(state);
	}
	
}

COpenZWaveControllerCache::DeviceCache & COpenZWaveControllerCache::getDeviceCache()
{
   return m_deviceCache;
}

COpenZWaveControllerCache::CDeviceInfoAndState & COpenZWaveControllerCache::getDeviceInfo(const std::string & deviceName)
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
