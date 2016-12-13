#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <boost/dynamic_bitset.hpp>
#include "../IType.h"
#include "../generated-eep.h"
#include "Profile_D2_01_Common.h"

namespace yApi = shared::plugin::yPluginApi;


class CProfile_D2_01_12 : public IType
{
public:
   CProfile_D2_01_12(const std::string& deviceId,
      boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_01_12();

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(unsigned char rorg,
      const boost::dynamic_bitset<>& data,
      const boost::dynamic_bitset<>& status) const override;
   void sendCommand(const std::string& keyword,
      const std::string& commandBody,
      const std::string& senderId,
      boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
      const std::string& senderId,
      boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   static const CRorgs::ERorgIds m_rorg;
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CSwitch> m_channel1;
   boost::shared_ptr<yApi::historization::CSwitch> m_channel2;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
