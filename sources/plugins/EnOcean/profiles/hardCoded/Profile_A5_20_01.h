#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>


class CProfile_A5_20_01 : public IType
{
public:
   CProfile_A5_20_01(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_20_01() = default;

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   void readInitialState(const std::string& senderId,
                         boost::shared_ptr<IMessageHandler> messageHandler) const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(unsigned char rorg,
                                                                                   const boost::dynamic_bitset<>& data,
                                                                                   const boost::dynamic_bitset<>&
                                                                                   status,
                                                                                   const std::string& senderId,
                                                                                   boost::shared_ptr<IMessageHandler>
                                                                                   messageHandler) const override;
   void sendCommand(const std::string& keyword,
                    const std::string& commandBody,
                    const std::string& senderId,
                    boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          const std::string& senderId,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   static void sendMessage(boost::shared_ptr<IMessageHandler> messageHandler,
                           const std::string& senderId,
                           const std::string& targetId,
                           const boost::dynamic_bitset<>& userData);

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CDimmable> m_currentValue;
   boost::shared_ptr<yApi::historization::CSwitch> m_serviceOn;
   boost::shared_ptr<yApi::historization::CSwitch> m_energyInputEnable;
   boost::shared_ptr<yApi::historization::CSwitch> m_energyStorageCharged;
   boost::shared_ptr<yApi::historization::CBatteryLevel> m_battery;
   boost::shared_ptr<yApi::historization::CSwitch> m_coverOpen;
   boost::shared_ptr<yApi::historization::CSwitch> m_temperatureFailure;
   boost::shared_ptr<yApi::historization::CSwitch> m_windowOpen;
   boost::shared_ptr<yApi::historization::CSwitch> m_actuatorObstructed;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CDimmable> m_valvePosition;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperatureSetPoint;
   boost::shared_ptr<yApi::historization::CTemperature> m_currentTemperatureFromExternalSensor;
   boost::shared_ptr<yApi::historization::CEvent> m_runInitSequence;
   boost::shared_ptr<yApi::historization::CEvent> m_liftSet;
   boost::shared_ptr<yApi::historization::CSwitch> m_summerMode;
   boost::shared_ptr<yApi::historization::CSwitch> m_setPointInverse;
   mutable boost::optional<bool> m_setPointModeIsTemperature;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
