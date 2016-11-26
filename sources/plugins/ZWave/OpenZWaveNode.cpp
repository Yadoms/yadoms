#include "stdafx.h"
#include "OpenZWaveNode.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include "OpenZWaveHelpers.h"

COpenZWaveNode::COpenZWaveNode()
   : m_homeId(0), m_nodeId(0), m_configuration(0,0)
{
   throw shared::exception::CException("This code is only here to make it build. This code should never be called");
}

COpenZWaveNode::COpenZWaveNode(const uint32 homeId, const uint8 nodeId)
   : m_homeId(homeId), m_nodeId(nodeId), m_configuration(homeId, nodeId)
{
}

COpenZWaveNode::~COpenZWaveNode()
{
}

void COpenZWaveNode::registerKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (value.GetGenre() != OpenZWave::ValueID::ValueGenre_Config)
   {
      if (m_keywords.find(keyword) == m_keywords.end())
         m_keywords[keyword] = COpenZWaveNodeKeywordFactory::createKeyword(value, m_homeId, m_nodeId, includeSystemKeywords);
   }
   else
   {
      m_configuration.registerConfiguration(value);
   }
}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNode::getKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);
   if (value.GetGenre() != OpenZWave::ValueID::ValueGenre_Config)
   {
      if (m_keywords.find(keyword) == m_keywords.end())
      {
         registerKeyword(value, includeSystemKeywords);
         if (m_keywords.find(keyword) != m_keywords.end())
         {
            return boost::shared_ptr<IOpenZWaveNodeKeyword>();
         }
      }
      return m_keywords[keyword];
   }
   else
   {
      return m_configuration.getConfigurationItem(value);
   }
}


boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNode::updateKeywordValue(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   boost::shared_ptr<IOpenZWaveNodeKeyword> kw = getKeyword(value, includeSystemKeywords);
   kw->updateValue(value);
   return kw->getLastKeywordValue();
}

bool COpenZWaveNode::sendCommand(const std::string& keyword, const std::string& commandData)
{
   if (m_keywords.find(keyword) != m_keywords.end())
      return m_keywords[keyword]->sendCommand(commandData);

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}

const boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNode::getLastKeywordValue(const std::string& keyword)
{
   if (m_keywords.find(keyword) != m_keywords.end())
      return m_keywords[keyword]->getLastKeywordValue();

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}


const bool COpenZWaveNode::match(const uint32 homeId, const uint8 nodeId)
{
   return m_homeId == homeId && m_nodeId == nodeId;
}

const uint32 COpenZWaveNode::getHomeId()
{
   return m_homeId;
}

const uint8 COpenZWaveNode::getNodeId()
{
   return m_nodeId;
}

shared::CDataContainer COpenZWaveNode::getConfigurationSchema()
{
   return m_configuration.generateConfigurationSchema();
}

shared::CDataContainer COpenZWaveNode::getConfigurationValues()
{
   return m_configuration.saveValuesToDatabase();
}

void COpenZWaveNode::setConfigurationValues(const shared::CDataContainer &configuration)
{
   m_configuration.setConfigurationValues(configuration);
}
