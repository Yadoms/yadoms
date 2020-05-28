#include "stdafx.h"
#include "Pf2Configuration.h"
#include <boost/lexical_cast.hpp>

CPf2Configuration::CPf2Configuration()
   :m_data(shared::CDataContainer::make())
{
}

CPf2Configuration::~CPf2Configuration()
{
}

void CPf2Configuration::initializeWith(const boost::shared_ptr<shared::CDataContainer>& data)
{
   m_data.initializeWith(data);
}

EPullResistance CPf2Configuration::PullResistanceState(int pin) const
{
   // Enum type, declare keys labels
   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
      ("Disable" , kDisable)
      ("Pullup"  , kPullUp);

   std::string name ( "DI" );
   name.append ( boost::lexical_cast<std::string>(pin) );
   return m_data->getEnumValue<EPullResistance>( name, EEnumTypeNames );
}
