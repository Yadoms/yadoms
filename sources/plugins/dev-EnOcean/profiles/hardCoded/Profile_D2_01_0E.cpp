#include "stdafx.h"
#include "Profile_D2_01_0E.h"

CProfile_D2_01_0E::CProfile_D2_01_0E()
   : m_historizers({})
{
}

CProfile_D2_01_0E::~CProfile_D2_01_0E()
{
}

unsigned int CProfile_D2_01_0E::id() const
{
   return 0x0E;
}

const std::string& CProfile_D2_01_0E::title() const
{
   static const std::string title("Micro smart plug with 1 channel, and metering capabilities");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0E::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0E::states(const boost::dynamic_bitset<>& data) const
{
   //TODO
   return m_historizers;
}

void CProfile_D2_01_0E::sendConfiguration(const shared::CDataContainer& deviceConfiguration) const
{
   //TODO
}
