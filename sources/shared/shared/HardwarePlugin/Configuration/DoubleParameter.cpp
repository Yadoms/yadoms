#include "stdafx.h"

#include "DoubleParameter.h"

CHardwarePluginConfigurationDoubleParameter::CHardwarePluginConfigurationDoubleParameter(const std::string& name, const std::string& description, double defaultValue)
   :CHardwarePluginConfigurationSimpleParameter<double>("double", name, description, defaultValue)
{
}

boost::shared_ptr<CHardwarePluginConfigurationParameter> CHardwarePluginConfigurationDoubleParameter::clone() const
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationDoubleParameter(getName(), getDescription(), m_value));
   return p;
}


