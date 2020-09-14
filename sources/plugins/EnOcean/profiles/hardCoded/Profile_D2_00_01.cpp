#include "stdafx.h"
#include "Profile_D2_00_01.h"
#include "../bitsetHelpers.hpp"
#include "profiles/eep.h"
#include "message/MessageHelpers.h"


CProfile_D2_00_01::CProfile_D2_00_01(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_api(api),
     m_deviceId(deviceId),
     m_temperatureMeasure(boost::make_shared<yApi::historization::CTemperature>("Temperature measure",
                                                                                yApi::EKeywordAccessMode::kGet)),
     m_temperatureSetPoint(boost::make_shared<yApi::historization::CTemperature>("Temperature set point",
                                                                                 yApi::EKeywordAccessMode::kGet)),
     m_fanSpeed(boost::make_shared<specificHistorizers::CFan4Speeds>("Fan speed",
                                                                     yApi::EKeywordAccessMode::kGet)),
     m_presence(boost::make_shared<yApi::historization::CSwitch>("Presence",
                                                                 yApi::EKeywordAccessMode::kGet)),
     m_displayFan(boost::make_shared<yApi::historization::CSwitch>("DisplayFan",
                                                                   yApi::EKeywordAccessMode::kGetSet)),
     m_displayPresence(boost::make_shared<yApi::historization::CSwitch>("DisplayPresence",
                                                                        yApi::EKeywordAccessMode::kGetSet)),
     m_dataToDisplayToScreen(boost::make_shared<specificHistorizers::CDataToDisplayToScreen>("DataToDisplay")),
     m_illuminationToDisplayToScreen(boost::make_shared<yApi::historization::CIllumination>("IlluminationToDisplay",
                                                                                            yApi::EKeywordAccessMode::
                                                                                            kGetSet)),
     m_percentage(boost::make_shared<yApi::historization::CDimmable>("Percentage",
                                                                     yApi::EKeywordAccessMode::kGetSet)),
     m_concentrationPpm(boost::make_shared<specificHistorizers::CConcentrationPpm>("ConcentrationPpm",
                                                                                   yApi::EKeywordAccessMode::kGetSet)),
     m_relativeHumidity(boost::make_shared<yApi::historization::CHumidity>("RelativeHumidity",
                                                                           yApi::EKeywordAccessMode::kGetSet)),
     m_displayUserNotification(boost::make_shared<yApi::historization::CSwitch>("DisplayUserNotification",
                                                                                yApi::EKeywordAccessMode::kGetSet)),
     m_displayWindow(boost::make_shared<yApi::historization::CSwitch>("DisplayWindow",
                                                                      yApi::EKeywordAccessMode::kGetSet)),
     m_displayDewPoint(boost::make_shared<yApi::historization::CSwitch>("DisplayDewPoint",
                                                                        yApi::EKeywordAccessMode::kGetSet)),
     m_displayCooling(boost::make_shared<yApi::historization::CSwitch>("DisplayCooling",
                                                                       yApi::EKeywordAccessMode::kGetSet)),
     m_displayHeating(boost::make_shared<yApi::historization::CSwitch>("DisplayHeating",
                                                                       yApi::EKeywordAccessMode::kGetSet)),
     m_historizers({
        m_temperatureMeasure, m_temperatureSetPoint, m_fanSpeed, m_presence,
        m_displayFan, m_displayPresence, m_dataToDisplayToScreen, m_illuminationToDisplayToScreen,
        m_percentage, m_concentrationPpm, m_relativeHumidity, m_displayUserNotification, m_displayWindow,
        m_displayDewPoint, m_displayCooling, m_displayHeating
     })
{
}

const std::string& CProfile_D2_00_01::profile() const
{
   static const std::string Profile("D2-00-01");
   return Profile;
}

const std::string& CProfile_D2_00_01::title() const
{
   static const std::string Title("Room Control Panel (RCP) - RCP with Temperature Measurement and Display (BI-DIR)");
   return Title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_00_01::allHistorizers() const
{
   return m_historizers;
}

void CProfile_D2_00_01::readInitialState(const std::string& senderId,
                                         boost::shared_ptr<IMessageHandler> messageHandler) const
{
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_00_01::states(
   unsigned char rorg,
   const boost::dynamic_bitset<>& data,
   const boost::dynamic_bitset<>& status,
   const std::string& senderId,
   boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto msgId = static_cast<EMsgId>(bitset_extract(data, 5, 3));

   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   switch (msgId)
   {
   case kRepeatedUserActionOnRcp:
      {
         const auto fanSpeedRawValue = bitset_extract(data, 1, 3);
         switch (fanSpeedRawValue)
         {
         case 1:
            m_fanSpeed->set(specificHistorizers::EFan4Speeds::kSpeed0);
            historizers.push_back(m_fanSpeed);
            break;
         case 2:
            m_fanSpeed->set(specificHistorizers::EFan4Speeds::kSpeed1);
            historizers.push_back(m_fanSpeed);
            break;
         case 3:
            m_fanSpeed->set(specificHistorizers::EFan4Speeds::kSpeed2);
            historizers.push_back(m_fanSpeed);
            break;
         case 4:
            m_fanSpeed->set(specificHistorizers::EFan4Speeds::kSpeed3);
            historizers.push_back(m_fanSpeed);
            break;
         case 5:
            m_fanSpeed->set(specificHistorizers::EFan4Speeds::kAuto);
            historizers.push_back(m_fanSpeed);
            break;
         default:
            YADOMS_LOG(error) << "Unsupported message received for profile " << profile() <<
               " : Fan speed=" << fanSpeedRawValue;
            return std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>();
         }

         const auto presenceRawValue = bitset_extract(data, 8, 3);
         switch (presenceRawValue)
         {
         case 1:
            m_presence->set(true);
            historizers.push_back(m_presence);
            break;
         case 2:
            m_presence->set(false);
            historizers.push_back(m_presence);
            break;
         default:
            break;
         }

         if (bitset_extract(data, 11, 5) == 0x05)
         {
            m_temperatureSetPoint->set(static_cast<double>(bitset_extract(data, 16, 16)) / 100.0);
            historizers.push_back(m_temperatureSetPoint);
         }

         break;
      }
   case kMeasurementResult:
      {
         if (bitset_extract(data, 16, 4) == 0)
         {
            m_temperatureSetPoint->set(
               static_cast<double>(bitset_extract(data, 20, 4) << 8 || bitset_extract(data, 8, 8)) / 100.0);
            historizers.push_back(m_temperatureSetPoint);
         }

         break;
      }

   default:
      break; // nothing to do with this message
   }

   return historizers;
}

void CProfile_D2_00_01::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    const std::string& senderId,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   // Set internal state
   if (keyword == m_displayFan->getKeyword())
      m_displayFan->setCommand(commandBody);
   else if (keyword == m_displayPresence->getKeyword())
      m_displayPresence->setCommand(commandBody);
   else if (keyword == m_dataToDisplayToScreen->getKeyword())
      m_dataToDisplayToScreen->setCommand(commandBody);
   else if (keyword == m_displayUserNotification->getKeyword())
      m_displayUserNotification->setCommand(commandBody);
   else if (keyword == m_displayWindow->getKeyword())
      m_displayWindow->setCommand(commandBody);
   else if (keyword == m_displayDewPoint->getKeyword())
      m_displayDewPoint->setCommand(commandBody);
   else if (keyword == m_displayCooling->getKeyword())
      m_displayCooling->setCommand(commandBody);
   else if (keyword == m_displayHeating->getKeyword())
      m_displayHeating->setCommand(commandBody);
   else
      return;


   // Send message
   boost::dynamic_bitset<> userData(5 * 8);

   bitset_insert(userData, 5, 3, kDisplayContent);

   enum EFanMode { kAuto = 0, kManual = 1 };
   if (m_displayFan->get())
   {
      switch (m_fanSpeed->get())
      {
      case specificHistorizers::EFan4Speeds::kAutoValue:
         bitset_insert(userData, 0, 1, kAuto);
         bitset_insert(userData, 1, 3, 1);
         break;
      case specificHistorizers::EFan4Speeds::kSpeed0Value:
         bitset_insert(userData, 0, 1, kManual);
         bitset_insert(userData, 1, 3, 1);
         break;
      case specificHistorizers::EFan4Speeds::kSpeed1Value:
         bitset_insert(userData, 0, 1, kManual);
         bitset_insert(userData, 1, 3, 2);
         break;
      case specificHistorizers::EFan4Speeds::kSpeed2Value:
         bitset_insert(userData, 0, 1, kManual);
         bitset_insert(userData, 1, 3, 3);
         break;
      case specificHistorizers::EFan4Speeds::kSpeed3Value:
         bitset_insert(userData, 0, 1, kManual);
         bitset_insert(userData, 1, 3, 4);
         break;
      default:
         YADOMS_LOG(error) << "Unsupported command to profile " << profile() <<
            " : Fan speed=" << m_fanSpeed->get();
         return;
      }
   }
   else
   {
      bitset_insert(userData, 0, 1, kAuto);
      bitset_insert(userData, 1, 3, 0);
   }

   bitset_insert(userData, 4, 1, 0); // No more data

   bitset_insert(userData, 8, 3, m_displayPresence->get() ? 1 : 2); // Presence

   switch (m_dataToDisplayToScreen->get())
   {
   case specificHistorizers::EDataToDisplayToScreen::kNoDisplayValue:
      bitset_insert(userData, 11, 5, 0);
      break;
   case specificHistorizers::EDataToDisplayToScreen::kRoomTemperatureDegreesValue:
      bitset_insert(userData, 11, 5, 1);
      bitset_insert(userData, 16, 16, static_cast<int>(m_temperatureMeasure->get() * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kRoomTemperatureFarenheitValue:
      bitset_insert(userData, 11, 5, 2);
      bitset_insert(userData, 16, 16, static_cast<int>(toFarenheit(m_temperatureMeasure->get()) * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kNominalTemperatureDegreesValue:
      bitset_insert(userData, 11, 5, 3);
      bitset_insert(userData, 16, 16, static_cast<int>(m_temperatureSetPoint->get() * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kNominalTemperatureFarenheitValue:
      bitset_insert(userData, 11, 5, 4);
      bitset_insert(userData, 16, 16, static_cast<int>(toFarenheit(m_temperatureSetPoint->get()) * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kDeltaTemperatureSetPointDegreesValue:
      bitset_insert(userData, 11, 5, 5);
      bitset_insert(userData, 16, 16,
                    static_cast<int>((m_temperatureMeasure->get() - m_temperatureSetPoint->get()) * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kDeltaTemperatureSetPointFarenheitValue:
      bitset_insert(userData, 11, 5, 6);
      bitset_insert(userData, 16, 16,
                    static_cast<int>(toFarenheit(
                       (m_temperatureMeasure->get() - m_temperatureSetPoint->get())) * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kDeltaTemperatureSetPointGraphicValue:
      bitset_insert(userData, 11, 5, 7);
      bitset_insert(userData, 16, 16,
                    static_cast<int>((m_temperatureMeasure->get() - m_temperatureSetPoint->get()) * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kIlluminationValue:
      bitset_insert(userData, 11, 5, 0x0d);
      bitset_insert(userData, 16, 16, static_cast<int>(m_illuminationToDisplayToScreen->get()));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kPercentageValue:
      bitset_insert(userData, 11, 5, 0x0e);
      bitset_insert(userData, 16, 16, static_cast<int>(m_percentage->get() * 100.0));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kPartsPerMillionValue:
      bitset_insert(userData, 11, 5, 0x0f);
      bitset_insert(userData, 16, 16, static_cast<int>(m_concentrationPpm->get()));
      break;
   case specificHistorizers::EDataToDisplayToScreen::kRelativeHumidityValue:
      bitset_insert(userData, 11, 5, 0x10);
      bitset_insert(userData, 16, 16, static_cast<int>(m_relativeHumidity->get() * 100.0));
      break;
   default:
      break;
   }

   bitset_insert(userData, 35, 1, m_displayUserNotification->get() ? 1 : 0);
   bitset_insert(userData, 36, 1, m_displayWindow->get() ? 1 : 0);
   bitset_insert(userData, 37, 1, m_displayDewPoint->get() ? 1 : 0);
   bitset_insert(userData, 38, 1, m_displayCooling->get() ? 1 : 0);
   bitset_insert(userData, 39, 1, m_displayHeating->get() ? 1 : 0);

   message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
                                         messageHandler,
                                         senderId,
                                         m_deviceId,
                                         userData,
                                         "send command");
}

void CProfile_D2_00_01::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          const std::string& senderId,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   const auto setPointRangeLimit =
      deviceConfiguration.get<bool>("setPointEnable.checkbox")
         ? deviceConfiguration.get<double>("setPointEnable.content.setPointRangeLimit")
         : 0.0;
   const auto setPointSteps =
      deviceConfiguration.get<bool>("setPointEnable.checkbox")
         ? deviceConfiguration.get<int>("setPointEnable.content.setPointSteps")
         : 0;
   const auto temperatureMeasurementTiming =
      deviceConfiguration.get<bool>("temperatureMeasurementEnable.checkbox")
         ? deviceConfiguration.get<int>("temperatureMeasurementEnable.content.measureInterval")
         : 0;
   const auto significantTemperatureDifference =
      deviceConfiguration.get<bool>("temperatureMeasurementEnable.checkbox")
         ? deviceConfiguration.get<double>("temperatureMeasurementEnable.content.significantDelta")
         : 0.0;
   const auto keepAliveTiming =
      deviceConfiguration.get<bool>("temperatureMeasurementEnable.checkbox")
         ? deviceConfiguration.get<int>("temperatureMeasurementEnable.content.keepAliveTiming")
         : 0;


   boost::dynamic_bitset<> data(6 * 8);

   bitset_insert(data, 4, 1, 0);
   bitset_insert(data, 5, 3, kSensorConfiguration);
   bitset_insert(data, 9, 7, static_cast<int>(setPointRangeLimit * 10.0));
   bitset_insert(data, 17, 7, setPointSteps);
   bitset_insert(data, 24, 4, (temperatureMeasurementTiming / 10) & 0x0F);
   bitset_insert(data, 32, 3, 1);
   bitset_insert(data, 35, 3, 4);
   bitset_insert(data, 38, 2, ((temperatureMeasurementTiming / 10) & 0x30) >> 4);
   bitset_insert(data, 40, 4, static_cast<int>(significantTemperatureDifference / 0.2));
   bitset_insert(data, 45, 3, keepAliveTiming / 10);

   message::CMessageHelpers::sendMessage(CRorgs::k4BS_Telegram,
                                         messageHandler,
                                         senderId,
                                         m_deviceId,
                                         data,
                                         "send configuration");
}

double CProfile_D2_00_01::toFarenheit(double celcius)
{
   return celcius * 9.0 / 5.0 + 32.0;
}
