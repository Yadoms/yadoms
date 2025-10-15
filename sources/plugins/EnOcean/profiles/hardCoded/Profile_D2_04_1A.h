#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>

#include "Profile_D2_04_Common.h"


class CProfile_D2_04_1A : public IType
{
public:
   CProfile_D2_04_1A(std::string deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   ~CProfile_D2_04_1A() override = default;

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   void readInitialState(const std::string& senderId,
                         boost::shared_ptr<IMessageHandler> messageHandler) override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(
      unsigned char rorg,
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
   CProfile_D2_04_Common m_common;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
