#pragma once
#include "../IType.h"


class CProfile_A5_12_00 : public IType
{
public:
   CProfile_A5_12_00(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_12_00();

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
   static const int NB_CHANNELS = 16;
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CCounter> m_cumulative[NB_CHANNELS];
   boost::shared_ptr<yApi::historization::CFrequency> m_currentValue[NB_CHANNELS];
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
