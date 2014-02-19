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

void CHardwarePluginConfigurationStringParameter::build(boost::property_tree::ptree& pt) const
{
   CHardwarePluginConfigurationParameter::build(pt);
   pt.put(getName() + ".default", m_defaultValue);
}

void CHardwarePluginConfigurationStringParameter::setValue(const boost::property_tree::ptree& pt)
{
   m_value = pt.get<std::string>(getName() + ".value");
}