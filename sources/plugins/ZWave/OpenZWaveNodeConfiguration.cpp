#include "stdafx.h"
#include "OpenZWaveNodeConfiguration.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include "OpenZWaveHelpers.h"
#include "ConfigurationSchemaFactory.h"
#include <shared/exception/NotSupported.hpp>
#include <shared/Log.h>

COpenZWaveNodeConfiguration::COpenZWaveNodeConfiguration(const uint32 homeId, const uint8 nodeId)
   : m_homeId(homeId), m_nodeId(nodeId)
{
}


COpenZWaveNodeConfiguration::~COpenZWaveNodeConfiguration()
{
}

void COpenZWaveNodeConfiguration::registerConfiguration(OpenZWave::ValueID& value)
{
   const std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (m_configurationItems.find(keyword) == m_configurationItems.end())
      m_configurationItems[keyword] = COpenZWaveNodeKeywordFactory::createKeyword(value, m_homeId, m_nodeId, false);

}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeConfiguration::getConfigurationItem(OpenZWave::ValueID& value)
{
   const std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (m_configurationItems.find(keyword) == m_configurationItems.end())
   {
      registerConfiguration(value);
      if (m_configurationItems.find(keyword) != m_configurationItems.end())
      {
         return boost::shared_ptr<IOpenZWaveNodeKeyword>();
      }
   }
   return m_configurationItems[keyword];
}

bool COpenZWaveNodeConfiguration::sendCommand(const std::string& keyword, const std::string& commandData)
{
   if (m_configurationItems.find(keyword) != m_configurationItems.end())
      return m_configurationItems[keyword]->sendCommand(commandData);

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}

shared::CDataContainerSharedPtr COpenZWaveNodeConfiguration::generateConfigurationSchema()
{
   std::map<int, std::string> orderedKeywordsByIndex;
   for (auto j = m_configurationItems.begin(); j != m_configurationItems.end(); ++j)
	   orderedKeywordsByIndex[j->second->getIndex()] = j->first;

   shared::CDataContainerSharedPtr result = new_CDataContainerSharedPtr();
   for (auto i = orderedKeywordsByIndex.begin(); i != orderedKeywordsByIndex.end(); ++i)
   {
	
      if (m_configurationItems[i->second] != nullptr)
      {
         try
         {
            auto itemSchema = CConfigurationSchemaFactory::generateForHistorizer(m_configurationItems[i->second]);
            result->set(CConfigurationSchemaFactory::generateValidKeyName(i->second), itemSchema);
         }
         catch (shared::exception::CNotSupported &)
         {
            YADOMS_LOG(information) << "Fail to generate configuration schema for : " << i->second << " : historizer not supported" ;
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(information) << "Exception in generating configuration schema for : " << i->second << " : " << ex.what() ;
         }
         
      }
   }

   return result;
}

shared::CDataContainerSharedPtr COpenZWaveNodeConfiguration::saveValuesToDatabase()
{
   shared::CDataContainerSharedPtr result = new_CDataContainerSharedPtr();
   for (auto i = m_configurationItems.begin(); i != m_configurationItems.end(); ++i)
   {
      if (i->second != nullptr)
      {
         try
         {
            auto itemSchema = CConfigurationSchemaFactory::generateForHistorizer(i->second);
            result->set(CConfigurationSchemaFactory::generateValidKeyName(i->first), i->second->getLastKeywordValue()->formatValue());
         }
         catch (shared::exception::CNotSupported &)
         {
            YADOMS_LOG(information) << "Fail to getting last value for : " << i->first << " : historizer not supported" ;
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(information) << "Exception in getting last value for : " << i->first << " : " << ex.what() ;
         }

      }
   }

   return result;
}

void COpenZWaveNodeConfiguration::setConfigurationValues(const shared::CDataContainerSharedPtr&configuration)
{
   shared::CDataContainerSharedPtr result = new_CDataContainerSharedPtr();
   for (auto i = m_configurationItems.begin(); i != m_configurationItems.end(); ++i)
   {
      if (i->second != nullptr)
      {
         try
         {
            auto keywordKey = CConfigurationSchemaFactory::generateValidKeyName(i->first);
            if (configuration->containsValue(keywordKey))
            {
				std::string currentValue = i->second->getLastKeywordValue()->formatValue();
				std::string newValue = configuration->get(keywordKey);
				if (!boost::iequals(currentValue, newValue))
				{
					YADOMS_LOG(information) << "Set configuration value for : " << i->first << " old=" << currentValue << " new=" << newValue;
					sendCommand(i->first, configuration->get<std::string>(keywordKey));
				}
				else
				{
					//ignore update, values are same
				}
               
            }
         }
         catch (shared::exception::CNotSupported &)
         {
            YADOMS_LOG(information) << "Fail to set configuration value for : " << i->first << " : historizer not supported" ;
         }
         catch (std::exception & ex)
         {
            YADOMS_LOG(information) << "Exception in setting configuration value for : " << i->first << " : " << ex.what() ;
         }

      }
   }
}

void COpenZWaveNodeConfiguration::updateNodeConfiguration(const std::string& keyword, const std::string& value, shared::CDataContainerSharedPtr& configuration)
{
   configuration->set(CConfigurationSchemaFactory::generateValidKeyName(keyword), value);
}