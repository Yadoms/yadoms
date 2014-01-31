#include "stdafx.h"

#include "BitsFieldListParameter.h"

CHardwarePluginConfigurationBitsFieldParameter::CHardwarePluginConfigurationBitsFieldParameter(const std::string& name, const std::string& description, const Items& items)
   :CHardwarePluginConfigurationParameter("bitsField", name, description), m_defaultValue(items), m_value(items)
{
}

boost::shared_ptr<CHardwarePluginConfigurationParameter> CHardwarePluginConfigurationBitsFieldParameter::clone() const
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationBitsFieldParameter(getName(), getDescription(), m_value));
   return p;
}

const std::string CHardwarePluginConfigurationBitsFieldParameter::valueToString(bool current) const
{
   //Function is not needed as getSchema was overridden, and "default" value overwritten
   return std::string();
}

void CHardwarePluginConfigurationBitsFieldParameter::valueFromString(const std::string& valueAsString)
{
   //Function is not needed as setValue was overridden
}

void CHardwarePluginConfigurationBitsFieldParameter::setValue(const boost::property_tree::ptree& pt)
{
   boost::property_tree::ptree itemsTree = pt.get_child(getName() + ".value");
   for (boost::property_tree::ptree::const_iterator itValue = itemsTree.begin() ; itValue != itemsTree.end() ; itValue++)
      m_value[itValue->first.data()] = itValue->second.data()=="true";
}

void CHardwarePluginConfigurationBitsFieldParameter::getSchema(boost::property_tree::ptree& pt) const
{
   CHardwarePluginConfigurationParameter::getSchema(pt);
   BOOST_FOREACH(Items::value_type item, m_defaultValue)
   {
      pt.put(getName() + ".default." + item.first, item.second);
   }
}
