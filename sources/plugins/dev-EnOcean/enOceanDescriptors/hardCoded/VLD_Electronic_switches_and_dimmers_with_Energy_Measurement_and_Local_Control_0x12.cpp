#include "VLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12.h"

CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12()
: m_historizers( {  } )
{}
CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::~CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12()
{}
unsigned int CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::id() const {
   return 0x12;
}
const std::string& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::title() const {
   static const std::string title("Type 0x12");
   return title;
}
const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::historizers() const {
   return m_historizers;
}
const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12::states(const boost::dynamic_bitset<>& data) const {
   return m_historizers;

}