#include "stdafx.h"

#include "SerialPortParameter.h"
#include "../../StringExtension.h"
#include <shared/Peripherals.h>

namespace shared { namespace plugin { namespace configuration
{

CSerialPortParameter::CSerialPortParameter(const std::string& name,
   const std::string& description)
   :CEnumGeneric(name, description, ValuesNames())
{
}

boost::shared_ptr<CParameter> CSerialPortParameter::clone() const
{
   CSerialPortParameter* parameter = new CSerialPortParameter(getName(), getDescription());
   parameter->m_value = get();
   boost::shared_ptr<CParameter> p(parameter);
   return p;
}

const std::string& CSerialPortParameter::get() const
{
   return m_value;
}

void CSerialPortParameter::setValue(const boost::property_tree::ptree& pt)
{
   m_value = pt.get<std::string>(getName() + ".value");
}

void CSerialPortParameter::build(boost::property_tree::ptree& pt) const
{
   // Update m_valuesNames before building schema
   // The normaly read-only m_valuesNames is here dynamic. So we need a write-access.
   ValuesNames* valuesNames = const_cast<ValuesNames*>(&m_valuesNames);
   valuesNames->clear();
   const boost::shared_ptr<const CPeripherals::SerialPortsMap> serialPorts(CPeripherals::getSerialPorts());
   int index=0;
   for (shared::CPeripherals::SerialPortsMap::const_iterator serialPort = serialPorts->begin() ; serialPort != serialPorts->end() ; ++serialPort)
      (*valuesNames)[index]=serialPort->first;

   // Normally build schema
   CEnumGeneric::build(pt);

   // Default value doesn't make sense here
   pt.put(getName() + ".default", CStringExtension::EmptyString);
}

} } } // namespace shared::plugin::configuration
