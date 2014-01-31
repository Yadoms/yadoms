#include "stdafx.h"

#include "Parameter.h"


CHardwarePluginConfigurationParameter::CHardwarePluginConfigurationParameter(const std::string& type, const std::string& name, const std::string& description)
   :m_type (type), m_name(name), m_description(description)
{
}

CHardwarePluginConfigurationParameter::~CHardwarePluginConfigurationParameter()
{
    
}

void CHardwarePluginConfigurationParameter::setValue(const boost::property_tree::ptree& pt)
{
   valueFromString(pt.get<std::string>(getName() + ".value"));
}

void CHardwarePluginConfigurationParameter::getSchema(boost::property_tree::ptree& pt) const
{
   pt.put(getName() + ".type", m_type);
   pt.put(getName() + ".description", m_description);
   pt.put(getName() + ".default", valueToString(false));
}
