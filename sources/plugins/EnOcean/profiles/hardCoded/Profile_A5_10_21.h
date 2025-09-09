#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include "specificHistorizers/SetPoint.h"
#include "specificHistorizers/SpecialHeatingStates.h"


class CProfile_A5_10_21 : public IType
{
public:
   CProfile_A5_10_21(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   ~CProfile_A5_10_21() override = default;

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
                                                                                   boost::shared_ptr<IMessageHandler> messageHandler) override;
   void sendCommand(const std::string& keyword,
                    const std::string& commandBody,
                    const std::string& senderId,
                    boost::shared_ptr<IMessageHandler> messageHandler) override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          const std::string& senderId,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   boost::shared_ptr<specificHistorizers::CSetPoint> m_setPoint;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CHumidity> m_humidity;
   boost::shared_ptr<specificHistorizers::CSpecialHeatingStates> m_heatingStates;
   boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;
   boost::shared_ptr<yApi::historization::CSwitch> m_userInteraction;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
