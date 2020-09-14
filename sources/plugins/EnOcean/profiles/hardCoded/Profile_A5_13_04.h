#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "specificHistorizers/DayOfWeek.h"


class CProfile_A5_13_04 : public IType
{
public:
   CProfile_A5_13_04(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_13_04() = default;

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
   static unsigned short convertTo24HFormat(unsigned short amPmHour,
                                            bool isPm);

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_deviceId;
   boost::shared_ptr<specificHistorizers::CDayOfWeek> m_dayOfWeek;
   boost::shared_ptr<yApi::historization::CDuration> m_time;
   boost::shared_ptr<yApi::historization::CSwitch> m_gpsSource;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
