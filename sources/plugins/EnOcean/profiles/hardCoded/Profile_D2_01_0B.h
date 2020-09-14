#pragma once
#include "../IType.h"


class CProfile_D2_01_0B : public IType
{
public:
   CProfile_D2_01_0B(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_01_0B() = default;

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
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CSwitch> m_channel;
   boost::shared_ptr<yApi::historization::CEnergy> m_loadEnergy;
   boost::shared_ptr<yApi::historization::CEvent> m_resetLoadEnergy;
   boost::shared_ptr<yApi::historization::CPower> m_loadPower;
   boost::shared_ptr<yApi::historization::CSwitch> m_powerFailure;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
