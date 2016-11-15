#include "stdafx.h"
#include "VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E.h"

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E()
   : m_historizers({})
{
}

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::~CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E()
{
}

unsigned int CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::id() const
{
   return 0x0E;
}

const std::string& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::title() const
{
   static const std::string title("Micro smart plug with 1 channel, and metering capabilities");
   return title;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::historizers() const
{
   return m_historizers;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0E::states(const boost::dynamic_bitset<>& data) const
{
   return m_historizers;
}

