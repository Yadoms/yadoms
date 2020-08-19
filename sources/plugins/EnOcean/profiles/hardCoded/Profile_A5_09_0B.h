#pragma once
#include "../IType.h"
#include <specificHistorizers/VolatileOrganicCompound.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include "specificHistorizers/VolumicRadioActivity.h"
#include "specificHistorizers/SieverPerSecondRadioActivity.h"
#include "specificHistorizers/MassRadioActivity.h"
#include "specificHistorizers/CountPerSecondRadioActivity.h"


class CProfile_A5_09_0B : public IType
{
public:
   CProfile_A5_09_0B(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_A5_09_0B() = default;

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
   boost::shared_ptr<yApi::historization::CVoltage> m_supplyVoltage;
   boost::shared_ptr<specificHistorizers::CSieverPerSecondRadioActivity> m_sieverPerSecondRadioActivity;
   boost::shared_ptr<specificHistorizers::CCountPerSecondRadioActivity> m_countPerSecondRadioActivity;
   boost::shared_ptr<specificHistorizers::CVolumicRadioActivity> m_volumicRadioActivity;
   boost::shared_ptr<specificHistorizers::CMassRadioActivity> m_massRadioActivity;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
