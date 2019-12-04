#include "stdafx.h"
#include "OpenZWaveNode.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include "OpenZWaveHelpers.h"
#include "OpenZWaveNodePluginFactory.h"

COpenZWaveNode::COpenZWaveNode()
   : m_homeId(0), m_nodeId(0), m_configuration(0,0)
{
   throw shared::exception::CException("This code is only here to make it build. This code should never be called");
}

COpenZWaveNode::COpenZWaveNode(const uint32 homeId, const uint8 nodeId)
   : m_homeId(homeId), m_nodeId(nodeId), m_configuration(homeId, nodeId)
{

   COpenZWaveNodePluginFactory::createPlugins(homeId, nodeId, m_plugins, this);
}

COpenZWaveNode::~COpenZWaveNode()
{
}

void COpenZWaveNode::registerKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   const std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (value.GetGenre() != OpenZWave::ValueID::ValueGenre_Config)
   {
      if (m_keywords.find(keyword) == m_keywords.end())
      {
         bool keywordHandledByPlugin = false;
         for (auto i = m_plugins.begin(); i != m_plugins.end(); ++i)
         {
            if ((*i)->isKeywordManagedByPlugin(value, m_homeId, m_nodeId))
            {
               keywordHandledByPlugin = true;
               const auto newKeyword = (*i)->createKeyword(value, m_homeId, m_nodeId, includeSystemKeywords);
               if (newKeyword)
                  m_keywords[keyword] = newKeyword;
               break;
            }
         }

         //in case not managed by plugin, use default factory
         if(!keywordHandledByPlugin)
            m_keywords[keyword] = COpenZWaveNodeKeywordFactory::createKeyword(value, m_homeId, m_nodeId, includeSystemKeywords);
      }
   }
   else
   {
      m_configuration.registerConfiguration(value);
   }
}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNode::getKeyword(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   const std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);
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


std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > COpenZWaveNode::updateKeywordValue(OpenZWave::ValueID& value, bool includeSystemKeywords)
{
   std::vector< boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> > result;

   boost::shared_ptr<IOpenZWaveNodeKeyword> kw = getKeyword(value, includeSystemKeywords);
   kw->updateValue(value);
   result.push_back(kw->getLastKeywordValue());

   //let plugins do their jobs
   for (auto i = m_plugins.begin(); i != m_plugins.end(); ++i)
   {
      if ((*i)->isKeywordManagedByPlugin(value, m_homeId, m_nodeId))
      {
         auto a = (*i)->onKeywordValueUpdated(value);
         for(auto k = a.begin(); k!=a.end();++k)
            result.push_back(*k);
      }
   }

   return result;
}

bool COpenZWaveNode::sendCommand(const std::string& keyword, const std::string& commandData)
{
   if (m_keywords.find(keyword) != m_keywords.end())
      return m_keywords[keyword]->sendCommand(commandData);

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}

boost::shared_ptr<shared::plugin::yPluginApi::historization::IHistorizable> COpenZWaveNode::getLastKeywordValue(const std::string& keyword)
{
   if (m_keywords.find(keyword) != m_keywords.end())
      return m_keywords[keyword]->getLastKeywordValue();

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}


bool COpenZWaveNode::match(const uint32 homeId, const uint8 nodeId) const
{
   return m_homeId == homeId && m_nodeId == nodeId;
}

uint32 COpenZWaveNode::getHomeId() const
{
   return m_homeId;
}

uint8 COpenZWaveNode::getNodeId() const
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

void COpenZWaveNode::updateNodeConfiguration(const std::string& keyword, const std::string& value, shared::CDataContainer & configuration)
{
   m_configuration.updateNodeConfiguration(keyword, value, configuration);
}

void COpenZWaveNode::getPluginExtraQueries(std::vector<std::string> & extraQueries)
{
   for (auto i = m_plugins.begin(); i != m_plugins.end(); ++i)
   {
      std::vector<std::string> l;
      (*i)->getExtraQueries(l);
      if (!l.empty())
      {
         for (auto j = l.begin(); j != l.end(); ++j)
         {
            std::string s = (boost::format("%1%_%2%") % (*i)->getName() % *j).str();
            extraQueries.push_back(s);
         }
      }
   }
}

bool COpenZWaveNode::onExtraQuery(const std::string & query, const shared::CDataContainer &data)
{
   //qeury must be: pluginName.extraQuery
   std::vector<std::string> result;
   boost::split(result, query, boost::is_any_of("_"), boost::token_compress_on);
   if (result.size() == 2)
   {
      for (auto i = m_plugins.begin(); i != m_plugins.end(); ++i)
      {
         if (boost::iequals(result[0], (*i)->getName()))
         {
            return (*i)->onExtraQuery(result[1], data);
         }
      }
   }
   return false;
}