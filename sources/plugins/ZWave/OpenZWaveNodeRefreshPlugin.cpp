#include "stdafx.h"
#include "OpenZWaveNodeRefreshPlugin.h"
#include "OpenZWaveCommandClass.h"
#include <Manager.h>
#include <command_classes/UserCode.h>
#include "OpenZWaveNodeKeywordFactory.h"
#include <boost/regex.hpp>
#include "OpenZWaveNode.h"

COpenZWaveNodeRefreshPlugin::COpenZWaveNodeRefreshPlugin(const uint32 homeId, const uint8 nodeId)
   :m_homeId(homeId), m_nodeId(nodeId)
{
}

COpenZWaveNodeRefreshPlugin::~COpenZWaveNodeRefreshPlugin()
{
}

bool COpenZWaveNodeRefreshPlugin::isKeywordManagedByPlugin(OpenZWave::ValueID& vID, const uint32 homeId, const uint8 nodeId)
{
   return false;
}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeRefreshPlugin::createKeyword(OpenZWave::ValueID& vID, uint32 homeId, uint8 nodeId, bool includeSystemKeywords)
{      
   return boost::shared_ptr<IOpenZWaveNodeKeyword>();
}


std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > COpenZWaveNodeRefreshPlugin::onKeywordValueUpdated(OpenZWave::ValueID& vID)
{
   std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > result;
   return result;
}

void COpenZWaveNodeRefreshPlugin::getExtraQueries(std::vector<std::string> & extraQueriesNames)
{
   extraQueriesNames.push_back("refresh");
}

std::string COpenZWaveNodeRefreshPlugin::getName()
{
   return "base";
}

bool COpenZWaveNodeRefreshPlugin::onExtraQuery(const std::string & query, const shared::CDataContainer &data)
{
   if (boost::iequals(query, "refresh"))
   {
      OpenZWave::Manager::Get()->RefreshNodeInfo(m_homeId, m_nodeId);
      return true;
   }
   return false;
}