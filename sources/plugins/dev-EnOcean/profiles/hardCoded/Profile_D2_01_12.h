#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <boost/dynamic_bitset.hpp>
#include "../IType.h"
#include "../generated-eep.h"

namespace yApi = shared::plugin::yPluginApi;


class CProfile_D2_01_12 : public IType
{
public:
   DECLARE_ENUM_HEADER(EDefaultState,
      ((off)(0))
      ((on)(1))
      ((previousState)(2))
      ((notUsed)(3))
   ) ;

   DECLARE_ENUM_HEADER(EConnectedSwitchsType,
      ((switch)(1))
      ((pushButton)(2))
      ((autodetection)(3))
   ) ;


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
                    boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   static const CRorgs::ERorgIds m_rorg;
   const std::string m_deviceId;
   boost::shared_ptr<yApi::historization::CSwitch> m_channel1;
   boost::shared_ptr<yApi::historization::CSwitch> m_channel2;
   boost::shared_ptr<yApi::historization::CSwitch> m_out1;
   boost::shared_ptr<yApi::historization::CSwitch> m_out2;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
