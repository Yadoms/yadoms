#pragma once
#include "../IType.h"
#include <specificHistorizers/ConcentrationPpm.h>
#include <specificHistorizers/ConcentrationGramPerMeterCube.h>
#include <specificHistorizers/VolatileOrganicCompound.h>


class CProfile_A5_09_0C : public IType
{
public:
   CProfile_A5_09_0C(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_0C() = default;

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   void readInitialState(const std::string& senderId,
                         boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(unsigned char rorg,
                                                                                   const boost::dynamic_bitset<>& data,
                                                                                   const boost::dynamic_bitset<>& status,
                                                                                   const std::string& senderId,
                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendCommand(const std::string& keyword,
                    const std::string& commandBody,
                    const std::string& senderId,
                    boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          const std::string& senderId,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   boost::shared_ptr<specificHistorizers::CVolatileOrganicCompound> m_volatileOrganicCompound;
   boost::shared_ptr<specificHistorizers::CConcentrationPpm> m_concentrationPpm;
   boost::shared_ptr<specificHistorizers::CConcentrationGramPerMeterCube> m_concentrationGM3;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
