#include "stdafx.h"

#include "IntParameter.h"

CHardwarePluginConfigurationIntParameter::CHardwarePluginConfigurationIntParameter(const std::string& name, const std::string& description, int defaultValue)
   :CHardwarePluginConfigurationSimpleParameter<int>("int", name, description, defaultValue)
{
}

boost::shared_ptr<CHardwarePluginConfigurationParameter> CHardwarePluginConfigurationIntParameter::clone() const
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationIntParameter(getName(), getDescription(), m_value));
   return p;
}


