#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>


class CProfile_D2_02_01 : public IType
{
   enum class EMsgId
   {
      kSensorMeasurement = 1,
      kSensorTestTrigger = 2,
      kActuatorSetMeasurement= 3,
      kSensorMeasurementQuery = 4
   };

   enum class EMeasurementType
   {
      kTemperature = 0,
      kIllumination = 1,
      kOccupancy = 2,
      kSmoke = 3
   };

public:
   CProfile_D2_02_01(std::string deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   ~CProfile_D2_02_01() override = default;

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
                    boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          const std::string& senderId,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   void sendSensorMeasurementQuery(EMeasurementType query,
                                   const std::string& senderId,
                                   boost::shared_ptr<IMessageHandler> messageHandler) const;
   
   const std::string m_deviceId;

   boost::shared_ptr<yApi::historization::CTemperature> m_temperature;
   boost::shared_ptr<yApi::historization::CIllumination> m_illumination;
   boost::shared_ptr<yApi::historization::CSwitch> m_smoke;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
