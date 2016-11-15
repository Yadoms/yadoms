#include "stdafx.h"
#include "VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F.h"

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F()
   : m_historizers({})
{
}

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::~CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F()
{
}

unsigned int CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::id() const
{
   return 0x0F;
}

const std::string& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::title() const
{
   static const std::string title("Slot-in module with 1 channel");
   return title;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::historizers() const
{
   return m_historizers;
}

const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x0F::states(const boost::dynamic_bitset<>& data) const
{
   return m_historizers;
}

