#include "stdafx.h"

#include "../../exception/BadConversion.hpp"
#include "../../exception/OutOfRange.hpp"
#include "../../StringExtension.h"
#include "Configuration.h"
#include "Factory.h"


namespace shared { namespace plugin { namespace configuration
{

CConfiguration::CConfiguration()
   :m_configurationSerializer(CFactory::createSerializer())
{

}

CConfiguration::CConfiguration(const CConfiguration& src, const std::string& configurationValues)
   :m_configurationSerializer(src.m_configurationSerializer)
{
   boost::lock_guard<boost::mutex> lock(m_parametersMapMutex);
   boost::lock_guard<boost::mutex> srcLock(src.m_parametersMapMutex);

   // Full copy of configuration schema
   for(CParametersMap::const_iterator itParameter = src.m_parametersMap.begin() ; itParameter != src.m_parametersMap.end() ; itParameter++)
   {
      AddParameter(itParameter->second->clone());
   }

   // Overwrite parameters with provided values
   setValues(configurationValues);
}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::buildSchema()
{
   boost::lock_guard<boost::mutex> lock(m_parametersMapMutex);
   doBuildSchema();
}

std::string CConfiguration::getSchema() const
{
   boost::lock_guard<boost::mutex> lock(m_parametersMapMutex);

   boost::property_tree::ptree pt;
   for(CParametersMap::const_iterator itParameter = m_parametersMap.begin() ; itParameter != m_parametersMap.end() ; itParameter++)
      itParameter->second->build(pt);
   return m_configurationSerializer->serialize(pt);
}

void CConfiguration::setValues(const std::string& serializedConfiguration)
{
   boost::property_tree::ptree pt;
   m_configurationSerializer->deserialize(serializedConfiguration, pt);

   boost::lock_guard<boost::mutex> lock(m_parametersMapMutex);

   for(CParametersMap::iterator itParameter = m_parametersMap.begin() ; itParameter != m_parametersMap.end() ; itParameter++)
   {
      try
      {
         itParameter->second->setValue(pt);
      }
      catch (boost::property_tree::ptree_bad_path&)
      {
         // If parameter was added since last plugin version, it has no value in the database.
         // So ignore it to let its default value.
      }
   }
}

void CConfiguration::AddParameter(boost::shared_ptr<CParameter> parameter)
{
   BOOST_ASSERT(m_parametersMap.find(parameter->getName()) == m_parametersMap.end());  // Item already exists
   m_parametersMap[parameter->getName()] = parameter;
}

void CConfiguration::AddParameter(CParameter* parameter)
{
   boost::shared_ptr<CParameter> parameterPtr(parameter);
   AddParameter(parameterPtr);
}

const CParameter& CConfiguration::operator[](const std::string& parameterName) const
{
   return *m_parametersMap.at(parameterName);
}

} } } // namespace shared::plugin::configuration
