#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "specificHistorizers/DayOfWeek.h"


class CProfile_A5_13_05 : public IType
{
public:
   CProfile_A5_13_05(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   ~CProfile_A5_13_05() override = default;

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   void readInitialState(const std::string& senderId,
                         boost::shared_ptr<IMessageHandler> messageHandler) override;
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
                          boost::shared_ptr<IMessageHandler> messageHandler) override;
   // [END] IType implementation

private:
   boost::shared_ptr<yApi::historization::CDirection> m_elevation;
   boost::shared_ptr<yApi::historization::CDirection> m_azimuth;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
