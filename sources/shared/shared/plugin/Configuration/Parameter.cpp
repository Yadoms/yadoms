#include "stdafx.h"

#include "Parameter.h"


CHardwarePluginConfigurationParameter::CHardwarePluginConfigurationParameter(const std::string& type, const std::string& name, const std::string& description)
   :m_type (type), m_name(name), m_description(description)
{
}

CHardwarePluginConfigurationParameter::~CHardwarePluginConfigurationParameter()
{
    
}

void CHardwarePluginConfigurationParameter::build(boost::property_tree::ptree& pt) const
{
   pt.put(getName() + ".type", m_type);
   pt.put(getName() + ".description", m_description);
}
