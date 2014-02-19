#include "stdafx.h"

#include "EnumGenericParameter.h"


CHardwarePluginConfigurationEnumGeneric::CHardwarePluginConfigurationEnumGeneric(const std::string& name, const std::string& description, const ValuesNames& valuesNames)
   :CHardwarePluginConfigurationParameter("enum", name, description), m_valuesNames(valuesNames)
{
}

CHardwarePluginConfigurationEnumGeneric::~CHardwarePluginConfigurationEnumGeneric()
{
}

void CHardwarePluginConfigurationEnumGeneric::build(boost::property_tree::ptree& pt) const
{
   CHardwarePluginConfigurationParameter::build(pt);
   BOOST_FOREACH(ValuesNames::value_type valueName, m_valuesNames)
   {
      pt.put(getName() + ".availableValues." + valueName.second, valueName.first);
   }
}
