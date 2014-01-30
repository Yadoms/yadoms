#include "stdafx.h"

#include "SerialPortParameter.h"
#include <shared/Peripherals.h>

CHardwarePluginConfigurationSerialPortParameter::CHardwarePluginConfigurationSerialPortParameter(const std::string& name,
   const std::string& description, const std::string& defaultValue)
   :CHardwarePluginConfigurationEnumGeneric(name, description, ValuesNames()), m_defaultValue(defaultValue), m_value(defaultValue)
{
}

boost::shared_ptr<CHardwarePluginConfigurationParameter> CHardwarePluginConfigurationSerialPortParameter::clone() const
{
   boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationSerialPortParameter(getName(), getDescription(), m_value));
   return p;
}

const std::string& CHardwarePluginConfigurationSerialPortParameter::get() const
{
   return m_value;
}

const std::string CHardwarePluginConfigurationSerialPortParameter::valueToString(bool current) const
{
   return current ? m_value : m_defaultValue;
}

void CHardwarePluginConfigurationSerialPortParameter::valueFromString(const std::string& valueAsString)
{
   m_value = valueAsString;
}

void CHardwarePluginConfigurationSerialPortParameter::getSchema(boost::property_tree::ptree& pt) const
{
   // Update m_valuesNames before building schema
   // The normaly read-only m_valuesNames is here dynamic. So we need a write-access.
   ValuesNames* valuesNames = const_cast<ValuesNames*>(&m_valuesNames);
   valuesNames->clear();
   boost::shared_ptr<CPeripherals::SerialPortsMap> serialPorts(CPeripherals::getSerialPorts());
   int index=0;
   BOOST_FOREACH(CPeripherals::SerialPortsMap::value_type serialPort, *serialPorts)
   {
      (*valuesNames)[index]=serialPort.first;
   }

   // Normally build schema
   CHardwarePluginConfigurationEnumGeneric::getSchema(pt);
}
