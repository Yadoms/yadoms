#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>


class CProfile_A5_13_02 : public IType
{
public:
   CProfile_A5_13_02(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   ~CProfile_A5_13_02() override = default;

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
   boost::shared_ptr<yApi::historization::CIllumination> m_sunWest;
   boost::shared_ptr<yApi::historization::CIllumination> m_sunNorthSouth;
   boost::shared_ptr<yApi::historization::CIllumination> m_sunEast;
   boost::shared_ptr<yApi::historization::CSwitch> m_southHemisphere;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
