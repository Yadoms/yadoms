

class CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12 : public IType  {
public:
   CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12();
   virtual ~CVLD_Electronic_switches_and_dimmers_with_Energy_Measurement_and_Local_Control_0x12();
public:
   unsigned int id() const override;
   const std::string& title() const override;
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& historizers() const override;
   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& states(const boost::dynamic_bitset<>& data) const override;
protected:
private:
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_historizers;
};