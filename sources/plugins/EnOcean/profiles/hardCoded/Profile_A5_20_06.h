#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/shared_ptr.hpp>


class CProfile_A5_20_06 : public IType
{
public:
    CProfile_A5_20_06(const std::string& deviceId,
                      boost::shared_ptr<yApi::IYPluginApi> api);
    ~CProfile_A5_20_06() override = default;

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
   DECLARE_ENUM_HEADER(ECommunicationInterval,
      ((auto)(0))
      ((_2_minutes)(1))
      ((_5_minutes)(2))
      ((_10_minutes)(3))
      ((_20_minutes)(4))
      ((_30_minutes)(5))
      ((_60_minutes)(6))
      ((_120_minutes)(7))
   ) ;

    void updatePendingCommand();
    [[nodiscard]] double byteRange0To80ToCelciusDegrees(const unsigned int byte);
    [[nodiscard]] double byteRange0To160ToCelciusDegrees(const unsigned int byte);
    [[nodiscard]] std::uint8_t celciusDegreesRange0To80ToByte(const double degrees);
    [[nodiscard]] std::uint8_t celciusDegreesRange0To160ToByte(const double degrees);
    [[nodiscard]] int localOffsetToCelciusDegreesInValvePositionMode(unsigned int lo);
    [[nodiscard]] int computeValvePosition(int currentValvePosition,
                                           int localOffsetInCelciusDegrees);

    const std::string m_deviceId;
    boost::shared_ptr<yApi::historization::CSwitch> m_energyInputEnable;
    boost::shared_ptr<yApi::historization::CBatteryLevel> m_battery;
    boost::shared_ptr<yApi::historization::CSwitch> m_windowOpen;
    boost::shared_ptr<yApi::historization::CSwitch> m_actuatorObstructed;
    boost::shared_ptr<yApi::historization::CTemperature> m_ambientTemperature;
    boost::shared_ptr<yApi::historization::CTemperature> m_feedTemperature;
    boost::shared_ptr<yApi::historization::CDimmable> m_valvePosition;
    boost::shared_ptr<yApi::historization::CTemperature> m_temperatureSetPoint;
    boost::shared_ptr<yApi::historization::CTemperature> m_externalSensorTemperature;
    boost::shared_ptr<yApi::historization::CSwitch> m_summerMode;
    mutable bool m_setPointModeIsTemperature = true;
    std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;

	bool m_useInternalSensor = true;
	bool m_communicationInterval = false;

    std::mutex m_pendingCommandMutex;
    boost::dynamic_bitset<> m_pendingCommand;
};
