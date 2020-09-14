#pragma once
#include "../IType.h"
#include "../../IMessageHandler.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include "specificHistorizers/Fan4Speeds.h"
#include "specificHistorizers/DataToDisplayToScreen.h"
#include "specificHistorizers/ConcentrationPpm.h"


class CProfile_D2_00_01 : public IType
{
   enum EMsgId
   {
      kFirstUserActionOnRcp = 1,
      kDisplayContent = 2,
      kRepeatedUserActionOnRcp = 3,
      kMeasurementResult = 4,
      kSensorConfiguration = 5
   };

public:
   CProfile_D2_00_01(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_D2_00_01() = default;

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
   static double toFarenheit(double celcius);

   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string m_deviceId;

   // Input devices
   boost::shared_ptr<yApi::historization::CTemperature> m_temperatureMeasure;
   boost::shared_ptr<yApi::historization::CTemperature> m_temperatureSetPoint;
   boost::shared_ptr<specificHistorizers::CFan4Speeds> m_fanSpeed;
   boost::shared_ptr<yApi::historization::CSwitch> m_presence;
   // Display
   boost::shared_ptr<yApi::historization::CSwitch> m_displayFan;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayPresence;
   boost::shared_ptr<specificHistorizers::CDataToDisplayToScreen> m_dataToDisplayToScreen;
   boost::shared_ptr<yApi::historization::CIllumination> m_illuminationToDisplayToScreen;
   boost::shared_ptr<yApi::historization::CDimmable> m_percentage;
   boost::shared_ptr<specificHistorizers::CConcentrationPpm> m_concentrationPpm;
   boost::shared_ptr<yApi::historization::CHumidity> m_relativeHumidity;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayUserNotification;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayWindow;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayDewPoint;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayCooling;
   boost::shared_ptr<yApi::historization::CSwitch> m_displayHeating;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
};
