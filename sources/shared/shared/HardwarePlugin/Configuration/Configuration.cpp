#include "stdafx.h"

#include "../../Exceptions/BadConversionException.hpp"
#include "../../Exceptions/OutOfRangeException.hpp"
#include "Configuration.h"
#include "ConfigurationFactory.h"


CHardwarePluginConfiguration::CHardwarePluginConfiguration()
   :m_configurationSerializer(CConfigurationFactory::createSerializer())
{

}

CHardwarePluginConfiguration::CHardwarePluginConfiguration(const CHardwarePluginConfiguration& src, const std::string& configurationValues)
   :m_configurationSerializer(src.m_configurationSerializer)
{
   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);
   boost::lock_guard<boost::mutex> srcLock(const_cast<boost::mutex&> (src.m_configurationMapMutex));

   // Full copy of configuration schema
   for(CHardwarePluginConfigurationMap::const_iterator itParameter = src.m_configurationMap.begin() ; itParameter != src.m_configurationMap.end() ; itParameter++)
   {
      AddParameter(itParameter->second->clone());
   }

   // Overwrite parameters with provided values
   setValues(configurationValues);
}

CHardwarePluginConfiguration::~CHardwarePluginConfiguration()
{
}

void CHardwarePluginConfiguration::buildSchema()
{
   boost::lock_guard<boost::mutex> lock(m_configurationMapMutex);
   doBuildSchema();
}

std::string CHardwarePluginConfiguration::getSchema() const
{
   boost::lock_guard<boost::mutex> lock(const_cast<boost::mutex&> (m_configurationMapMutex));

   boost::property_tree::ptree pt;
   for(CHardwarePluginConfigurationMap::const_iterator itParameter = m_configurationMap.begin() ; itParameter != m_configurationMap.end() ; itParameter++)
      itParameter->second->build(pt);
   return m_configurationSerializer->serialize(pt);
}

void CHardwarePluginConfiguration::setValues(const std::string& serializedConfiguration)
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

void CHardwarePluginConfiguration::AddParameter(boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter)
{
   BOOST_ASSERT(m_configurationMap.find(parameter->getName()) == m_configurationMap.end());  // Item already exists
   m_configurationMap[parameter->getName()] = parameter;
}

void CHardwarePluginConfiguration::AddParameter(CHardwarePluginConfigurationParameter* parameter)
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> parameterPtr(parameter);
   AddParameter(parameterPtr);
}

const CHardwarePluginConfigurationParameter& CHardwarePluginConfiguration::operator[](const std::string& parameterName) const
{
   return *m_configurationMap.at(parameterName);
}

bool CHardwarePluginConfiguration::asBool(const std::string& parameterName) const
{
   try
   {
      return (dynamic_cast<const CHardwarePluginConfigurationBoolParameter&>
         (*m_configurationMap.at(parameterName))).get();
   }
   catch (std::bad_cast&)
   {
      throw CBadConversionException(parameterName, "bool");
   }
}

const std::string& CHardwarePluginConfiguration::asString(const std::string& parameterName) const
{
   try
   {
      return (dynamic_cast<const CHardwarePluginConfigurationStringParameter&>
         (*m_configurationMap.at(parameterName))).get();
   }
   catch (std::bad_cast&)
   {
      throw CBadConversionException(parameterName, "bool");
   }
}

const std::string& CHardwarePluginConfiguration::asSerialPort(const std::string& parameterName) const
{
   try
   {
      return (dynamic_cast<const CHardwarePluginConfigurationSerialPortParameter&>
         (*m_configurationMap.at(parameterName))).get();
   }
   catch (std::bad_cast&)
   {
      throw CBadConversionException(parameterName, "bool");
   }
}

CHardwarePluginConfigurationBitsFieldParameter::Items CHardwarePluginConfiguration::asBitsField(const std::string& parameterName) const
{
   try
   {
      return (dynamic_cast<const CHardwarePluginConfigurationBitsFieldParameter&>
         (*m_configurationMap.at(parameterName))).get();
   }
   catch (std::bad_cast&)
   {
      throw CBadConversionException(parameterName, "bits field");
   }
}