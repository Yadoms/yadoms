#include "stdafx.h"

#include "StringParameter.h"

CHardwarePluginConfigurationStringParameter::CHardwarePluginConfigurationStringParameter(const std::string& name,
   const std::string& description, const std::string& defaultValue)
   :CHardwarePluginConfigurationParameter("string", name, description), m_defaultValue(defaultValue), m_value(defaultValue)
{
}

boost::shared_ptr<CHardwarePluginConfigurationParameter> CHardwarePluginConfigurationStringParameter::clone() const
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationStringParameter(getName(), getDescription(), m_value));
   return p;
}

const std::string& CHardwarePluginConfigurationStringParameter::get() const
{
   return m_value;
}

const std::string CHardwarePluginConfigurationStringParameter::valueToString(bool current) const
{
   return current ? m_value : m_defaultValue;
}

void CHardwarePluginConfigurationStringParameter::valueFromString(const std::string& valueAsString)
{
   m_value = valueAsString;
}
