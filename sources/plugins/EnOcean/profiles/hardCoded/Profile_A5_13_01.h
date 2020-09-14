#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "Profile_A5_13_02.h"
#include "Profile_A5_13_06.h"

// Be careful, this device has a specific behaviour, as said by profile documentation :
//    "A receiver that accepts EEP A5-13-01 at teach-in automatically needs to accept telegrams from
//    the same ID that comply to the definitions of EEP A5-13-02 thru EEP A5-13-06. Different
//    telegrams received from that ID need to be distinguished by their 4 bit identifiers."
//
// The plugin registers the device from the paired profile. It means that all incoming messages
// for this device produce a call of this function. We have to redirect message processing
// to the right profile, defined by the Identifier value (data at bits 24-28)
//
class CProfile_A5_13_01 : public IType
{
public:
   CProfile_A5_13_01(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_01() = default;

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
   boost::shared_ptr<yApi::historization::CIllumination> m_dawn;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CSpeed> m_windSpeed;
   boost::shared_ptr<yApi::historization::CSwitch> m_dayLight;
   boost::shared_ptr<yApi::historization::CSwitch> m_rain;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;

   CProfile_A5_13_02 m_subProfile_A5_13_02;
   CProfile_A5_13_06 m_subProfile_A5_13_06;
};
