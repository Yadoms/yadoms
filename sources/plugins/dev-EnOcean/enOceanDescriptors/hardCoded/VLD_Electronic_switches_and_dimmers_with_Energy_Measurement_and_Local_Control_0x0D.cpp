#include "stdafx.h"
#include "VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D.h"

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D()
   : m_historizers({})
{
}

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::~CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D()
{
}

unsigned int CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::id() const
{
   return 0x0D;
}

const std::string& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::title() const
{
   static const std::string title("Micro smart plug with 1 channel");
   return title;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::historizers() const
{
   return m_historizers;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0D::states(const boost::dynamic_bitset<>& data) const
{
   return m_historizers;
}

