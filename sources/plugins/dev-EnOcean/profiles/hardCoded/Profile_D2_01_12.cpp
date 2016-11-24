#include "stdafx.h"
#include "Profile_D2_01_12.h"
#include "profiles/bitsetHelpers.hpp"
#include "message/RadioErp1SendMessage.h"

DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_D2_01_12::EDefaultState, EDefaultState,
   ((off))
   ((on))
   ((previousState))
   ((notUsed))
);

DECLARE_ENUM_IMPLEMENTATION_NESTED(CProfile_D2_01_12::EConnectedSwitchsType, EConnectedSwitchsType,
   ((switch))
   ((pushButton))
   ((autodetection))
);

CProfile_D2_01_12::CProfile_D2_01_12(const std::string& deviceId,
                                     boost::shared_ptr<yApi::IYPluginApi> api)
   : m_deviceId(deviceId),
     m_switch1(boost::make_shared<yApi::historization::CSwitch>("Switch 1")),
     m_switch2(boost::make_shared<yApi::historization::CSwitch>("Switch 2")),
     m_historizers({m_switch1 , m_switch2})
{
}

CProfile_D2_01_12::~CProfile_D2_01_12()
{
}

const std::string& CProfile_D2_01_12::profile() const
{
   static const std::string profile("D2-01-12");
   return profile;
}

const std::string& CProfile_D2_01_12::title() const
{
   static const std::string title("Slot-in module with 2 channels");
   return title;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_12::allHistorizers() const
{
   return m_historizers;
}

std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> CProfile_D2_01_12::states(const boost::dynamic_bitset<>& data,
                                                                                                   const boost::dynamic_bitset<>& status) const
{
   // Return only the concerned historizer
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> historizers;

   auto ioChannel = bitset_extract(data, 11, 5);
   auto state = bitset_extract(data, 17, 1) ? true : false;
   switch (ioChannel)
   {
   case 0:
      m_switch1->set(state);
      historizers.push_back(m_switch1);
      break;
   case 1:
      m_switch2->set(state);
      historizers.push_back(m_switch2);
      break;
   default:
      std::cout << "Profile " << profile() << " : received unsupported ioChannel value " << ioChannel << std::endl;
      break;
   }
   return historizers;
}

void CProfile_D2_01_12::sendCommand(const std::string& keyword,
                                    const std::string& commandBody,
                                    boost::shared_ptr<IMessageHandler> messageHandler) const
{
   throw std::logic_error("device supports no command sending");
}

void CProfile_D2_01_12::sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                                          boost::shared_ptr<IMessageHandler> messageHandler) const
{
   auto localControl = deviceConfiguration.get<std::string>("localControl") == "enable";
   auto taughtInAllDevices = deviceConfiguration.get<std::string>("taughtIn") == "allDevices";
   auto userInterfaceDayMode = deviceConfiguration.get<std::string>("userInterfaceMode") == "dayMode";
   auto defaultState = deviceConfiguration.get<EDefaultState>("defaultState");
   auto connectedSwitchsType = deviceConfiguration.get<EConnectedSwitchsType>("connectedSwitchsType");
   auto autoOffTimerSeconds = deviceConfiguration.get<unsigned int>("autoOffTimer");
   auto delayRadioOffTimerSeconds = deviceConfiguration.get<unsigned int>("delayRadioOffTimer");
   auto switchingStateToggle = deviceConfiguration.get<std::string>("switchingState") == "tooggle";

   // CMD 0x2 - Actuator Set Local
   {
      message::CRadioErp1SendMessage command;
      boost::dynamic_bitset<> data(4 * 8);

      bitset_insert(data, 4, 4, 0x02); // CMD 0x2 - Actuator Set Local
      bitset_insert(data, 0, !taughtInAllDevices);
      bitset_insert(data, 10, localControl);
      bitset_insert(data, 11, 5, 0x1E); // Use all output channels supported by the device
      bitset_insert(data, 24, !userInterfaceDayMode);
      bitset_insert(data, 26, 2, defaultState);

      command.appendData(bitset_to_bytes(data));

      boost::shared_ptr<const message::CEsp3ReceivedPacket> answer;
      if (!messageHandler->send(command,
                                [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                {
                                   return esp3Packet->header().packetType() == message::RESPONSE;
                                },
                                [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                {
                                   answer = esp3Packet;
                                }))
         std::cerr << "Fail to send configuration to " << m_deviceId << " : no answer to Actuator Set Local command" << std::endl;

      if (answer->data()[0] != message::RET_OK)
         std::cerr << "Fail to send configuration to " << m_deviceId << " : Actuator Set Local command returns " << answer->data()[0] << std::endl;
   }

   // CMD 0xB - Actuator Set External Interface Settings
   {
      message::CRadioErp1SendMessage command;
      boost::dynamic_bitset<> data(7 * 8);

      bitset_insert(data, 4, 4, 0x0B); // CMD 0xB - Actuator Set External Interface Settings
      bitset_insert(data, 11, 5, 0x1E); // Use all output channels supported by the device
      bitset_insert(data, 16, 16, autoOffTimerSeconds);
      bitset_insert(data, 32, 16, delayRadioOffTimerSeconds);
      bitset_insert(data, 48, 2, connectedSwitchsType);
      bitset_insert(data, 50, !switchingStateToggle);

      command.appendData(bitset_to_bytes(data));

      boost::shared_ptr<const message::CEsp3ReceivedPacket> answer;
      if (!messageHandler->send(command,
                                [](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                {
                                   return esp3Packet->header().packetType() == message::RESPONSE;
                                },
                                [&](boost::shared_ptr<const message::CEsp3ReceivedPacket> esp3Packet)
                                {
                                   answer = esp3Packet;
                                }))
         std::cerr << "Fail to send configuration to " << m_deviceId << " : no answer to Actuator Set External Interface Settings command" << std::endl;

      if (answer->data()[0] != message::RET_OK)
         std::cerr << "Fail to send configuration to " << m_deviceId << " : Actuator Set External Interface Settings command returns " << answer->data()[0] << std::endl;
   }
}
