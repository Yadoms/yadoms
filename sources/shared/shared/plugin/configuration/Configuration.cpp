#include "stdafx.h"

#include "../../Exceptions/BadConversionException.hpp"
#include "../../Exceptions/OutOfRangeException.hpp"
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
   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);
   boost::lock_guard<boost::mutex> srcLock(src.m_configurationMapMutex);

   // Full copy of configuration schema
   for(CHardwarePluginConfigurationMap::const_iterator itParameter = src.m_configurationMap.begin() ; itParameter != src.m_configurationMap.end() ; itParameter++)
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
   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);
   doBuildSchema();
}

std::string CConfiguration::getSchema() const
{
   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);

   boost::property_tree::ptree pt;
   for(CHardwarePluginConfigurationMap::const_iterator itParameter = m_configurationMap.begin() ; itParameter != m_configurationMap.end() ; itParameter++)
      itParameter->second->build(pt);
   return m_configurationSerializer->serialize(pt);
}

void CConfiguration::setValues(const std::string& serializedConfiguration)
{
   boost::property_tree::ptree pt;
   m_configurationSerializer->deserialize(serializedConfiguration, pt);

   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);

   for(CHardwarePluginConfigurationMap::iterator itParameter = m_configurationMap.begin() ; itParameter != m_configurationMap.end() ; itParameter++)
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
   BOOST_ASSERT(m_configurationMap.find(parameter->getName()) == m_configurationMap.end());  // Item already exists
   m_configurationMap[parameter->getName()] = parameter;
}

void CConfiguration::AddParameter(CParameter* parameter)
{
   boost::shared_ptr<CParameter> parameterPtr(parameter);
   AddParameter(parameterPtr);
}

const CParameter& CConfiguration::operator[](const std::string& parameterName) const
{
   return *m_configurationMap.at(parameterName);
}

} } } // namespace shared::plugin::configuration
