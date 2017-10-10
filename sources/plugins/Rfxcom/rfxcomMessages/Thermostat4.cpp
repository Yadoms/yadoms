#include "stdafx.h"
#include "Thermostat4.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat4::CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& keyword,
                              const std::string& command,
                              const shared::CDataContainer& deviceDetails)
      : m_onOff(boost::make_shared<yApi::historization::CSwitch>("onOff")),
        m_flame(boost::make_shared<yApi::historization::CDimmable>("flame")),
        m_fan1(boost::make_shared<yApi::historization::CDimmable>("fan 1")),
        m_fan1AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 1 auto mode")),
        m_fan2(boost::make_shared<yApi::historization::CDimmable>("fan 2")),
        m_fan2AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 2 auto mode")),
        m_fan3(boost::make_shared<yApi::historization::CDimmable>("fan 3")),
        m_fan3AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 3 auto mode")),
        m_keywords({m_onOff, m_flame, m_fan1, m_fan1AutoMode, m_fan2, m_fan2AutoMode, m_fan3, m_fan3AutoMode})
   {
      if (boost::iequals(keyword, m_onOff->getKeyword()))
      {
         m_onOff->setCommand(command);
         m_flame->set(0);
      }
      else if (boost::iequals(keyword, m_flame->getKeyword()))
      {
         m_onOff->set(true);
         m_flame->set(command);
      }
      else
      {
         YADOMS_LOG(error) << "Unsupported keyword \"" << keyword << "\" for CThermostat4 device ";
         return;
      }

      m_unitCode = deviceDetails.get<unsigned int>("unitCode");

      Init(api);
   }

   CThermostat4::CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const std::string& name,
                              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_onOff(boost::make_shared<yApi::historization::CSwitch>("onOff")),
        m_flame(boost::make_shared<yApi::historization::CDimmable>("flame")),
        m_fan1(boost::make_shared<yApi::historization::CDimmable>("fan 1")),
        m_fan1AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 1 auto mode")),
        m_fan2(boost::make_shared<yApi::historization::CDimmable>("fan 2")),
        m_fan2AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 2 auto mode")),
        m_fan3(boost::make_shared<yApi::historization::CDimmable>("fan 3")),
        m_fan3AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 3 auto mode")),
        m_keywords({m_onOff, m_flame, m_fan1, m_fan1AutoMode})
   {
      m_onOff->set(false);
      m_flame->set(0);

      m_subType = static_cast<unsigned char>(subType);

      if (m_subType != sTypeMCZ2)
      {
         m_keywords.push_back(m_fan2);
         m_keywords.push_back(m_fan2AutoMode);
      }

      if (m_subType != sTypeMCZ3)
      {
         m_keywords.push_back(m_fan3);
         m_keywords.push_back(m_fan3AutoMode);
      }

      if (m_subType != sTypeMCZ1 && m_subType != sTypeMCZ2 && m_subType != sTypeMCZ3)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned int>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CThermostat4::CThermostat4(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_onOff(boost::make_shared<yApi::historization::CSwitch>("onOff")),
        m_flame(boost::make_shared<yApi::historization::CDimmable>("flame")),
        m_fan1(boost::make_shared<yApi::historization::CDimmable>("fan 1")),
        m_fan1AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 1 auto mode")),
        m_fan2(boost::make_shared<yApi::historization::CDimmable>("fan 2")),
        m_fan2AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 2 auto mode")),
        m_fan3(boost::make_shared<yApi::historization::CDimmable>("fan 3")),
        m_fan3AutoMode(boost::make_shared<yApi::historization::CSwitch>("fan 3 auto mode")),
        m_keywords({m_onOff, m_flame})
   {
      m_subType = 0;
      m_unitCode = 0;

      // Should not be called (transmitter-only device)
      throw std::logic_error("Constructing CThermostat4 object from received buffer is not possible, CThermostat4 is transmitter-only device");
   }

   CThermostat4::~CThermostat4()
   {
   }

   void CThermostat4::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeThermostat4);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("unitCode", m_unitCode);
      }
   }

   void CThermostat4::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         m_deviceDetails.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CThermostat4::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.THERMOSTAT4);

      rbuf.THERMOSTAT4.packetlength = ENCODE_PACKET_LENGTH(LIGHTING3);
      rbuf.THERMOSTAT4.packettype = pTypeThermostat4;
      rbuf.THERMOSTAT4.subtype = m_subType;
      rbuf.THERMOSTAT4.seqnbr = seqNumberProvider->next();
      rbuf.THERMOSTAT4.unitcode1 = static_cast<unsigned char>(0xFF & (m_unitCode >> 16));
      rbuf.THERMOSTAT4.unitcode2 = static_cast<unsigned char>(0xFF & (m_unitCode >> 8));
      rbuf.THERMOSTAT4.unitcode3 = static_cast<unsigned char>(0xFF & m_unitCode);
      rbuf.THERMOSTAT4.beep = 0;
      rbuf.THERMOSTAT4.fan1_speed = m_fan1AutoMode->get() ? 6 : normalizeFanSpeed(m_fan1->switchLevel());
      rbuf.THERMOSTAT4.fan2_speed = m_subType == sTypeMCZ1 ? 1 : (m_fan2AutoMode->get() ? 6 : normalizeFanSpeed(m_fan2->switchLevel()));
      rbuf.THERMOSTAT4.fan3_speed = (m_subType == sTypeMCZ1 || m_subType == sTypeMCZ2) ? 1 : (m_fan3AutoMode->get() ? 6 : normalizeFanSpeed(m_fan3->switchLevel()));
      rbuf.THERMOSTAT4.flame_power = (m_flame->switchLevel() == 100) ? 5 : static_cast<BYTE>(floor(m_flame->switchLevel() / 20)) + 1;
      rbuf.THERMOSTAT4.mode = m_onOff->get() ? thermostat4_sManual : thermostat4_sOff;
      rbuf.THERMOSTAT4.rssi = 0;
      rbuf.THERMOSTAT4.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING3));
   }

   void CThermostat4::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CThermostat4::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CThermostat4::keywords()
   {
      return m_keywords;
   }

   void CThermostat4::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << m_unitCode;
      m_deviceName = ssdeviceName.str();
   }

   void CThermostat4::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeMCZ1: ssModel << "MCZ Pellet Stove (1 fan model)";
      case sTypeMCZ2: ssModel << "MCZ Pellet Stove (2 fans model)";
      case sTypeMCZ3: ssModel << "MCZ Pellet Stove (3 fans model)";
         break;
      default: ssModel << std::to_string(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CThermostat4::normalizeFanSpeed(int dimmableValue)
   {
      return static_cast<unsigned char>(dimmableValue * 5 / 100 + 1);
   }
} // namespace rfxcomMessages
