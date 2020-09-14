#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"


class CProfile_A5_11_03 : public IType
{
public:
   CProfile_A5_11_03(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_11_03() = default;

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
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CDimmable> m_blindPosition;
   boost::shared_ptr<yApi::historization::CDirection> m_angle;
   boost::shared_ptr<yApi::historization::CSwitch> m_errorState;
   boost::shared_ptr<yApi::historization::CText> m_errorMessage;
   boost::shared_ptr<yApi::historization::CCurtain> m_curtain;
   boost::shared_ptr<yApi::historization::CCurtain> m_currentMovement;
   boost::shared_ptr<yApi::historization::CSwitch> m_serviceMode;
   boost::shared_ptr<yApi::historization::CSwitch> m_inverseMode;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
