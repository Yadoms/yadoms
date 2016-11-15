#include "stdafx.h"
#include "Profile_D2_01_0D.h"

CProfile_D2_01_0D::CProfile_D2_01_0D()
   : m_historizers({})
{
}

CProfile_D2_01_0D::~CProfile_D2_01_0D()
{
}

unsigned int CProfile_D2_01_0D::id() const
{
   return 0x0D;
}

const std::string& CProfile_D2_01_0D::title() const
{
   static const std::string title("Micro smart plug with 1 channel");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0D::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_0D::states(const boost::dynamic_bitset<>& data) const
{
   //TODO
   return m_historizers;
}

