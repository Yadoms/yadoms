#include "stdafx.h"
#include "RFXSensor.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRFXSensor::CRFXSensor(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize,
                          boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
      : m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
        m_keywords({m_rssi})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeRFXSensor,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(RFXSENSOR),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.RFXSENSOR.subtype;

      m_id = rbuf.RFXSENSOR.id;

      switch (rbuf.RFXSENSOR.subtype)
      {
      case sTypeRFXSensorTemp:
         {
            m_temperature = boost::make_shared<yApi::historization::CTemperature>("temperature");
            m_keywords.push_back(m_temperature);

            auto temperature = ((rbuf.RFXSENSOR.msg1 & 0x7F) << 8 | rbuf.RFXSENSOR.msg2);
            if (rbuf.RFXSENSOR.msg1 & 0x80)
               temperature = -temperature;
            m_temperature->set(temperature / 100);
            break;
         }
      case sTypeRFXSensorAD:
         {
            m_adVoltage = boost::make_shared<yApi::historization::CVoltage>("adVoltage");
            m_keywords.push_back(m_temperature);

            m_adVoltage->set((rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2) / 1000);
            break;
         }
      case sTypeRFXSensorVolt:
         {
            m_voltage = boost::make_shared<yApi::historization::CVoltage>("voltage");
            m_keywords.push_back(m_temperature);

            m_voltage->set((rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2) / 1000);
            break;
         }
      case sTypeRFXSensorMessage:
         {
            processMessage(rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2);
            break;
         }
      default:
         {
            std::cout << "Unsupported RFXSensor sub-type " << rbuf.RFXSENSOR.subtype << std::endl;
            break;
         }
      }

      m_rssi->set(NormalizeRssiLevel(rbuf.RFXSENSOR.rssi));

      Init(api);
   }

   CRFXSensor::~CRFXSensor()
   {
   }

   void CRFXSensor::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeRFXSensor);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRFXSensor::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("RFXSensor is a read-only message, can not be encoded");
   }

   void CRFXSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CRFXSensor::getDeviceName() const
   {
      return m_deviceName;
   }

   void CRFXSensor::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CRFXSensor::buildDeviceModel()
   {
      m_deviceModel = "RFXSensor";
   }

   void CRFXSensor::processMessage(unsigned int msgId)
   {
      switch (msgId)
      {
      case 0x0001: std::cout << "RFXSensor received message \"Sensor addresses incremented\"" << std::endl;
         break;
      case 0x0002: std::cout << "RFXSensor received message \"Battery low detected\"" << std::endl;
         break;
      case 0x0081: std::cout << "RFXSensor received message \"No 1-wire device connected\"" << std::endl;
         break;
      case 0x0082: std::cout << "RFXSensor received message \"1-Wire ROM CRC error\"" << std::endl;
         break;
      case 0x0083: std::cout << "RFXSensor received message \"1-Wire device connected is not a DS18B20 or DS2438\"" << std::endl;
         break;
      case 0x0084: std::cout << "RFXSensor received message \"No end of read signal received from 1-Wiredevice\"" << std::endl;
         break;
      case 0x0085: std::cout << "RFXSensor received message \"1-Wire scratchpad CRC error\"" << std::endl;
         break;
      default: std::cout << "RFXSensor invalid received message (unknown ID " << msgId << ")" << std::endl;
         break;
      }
   }
} // namespace rfxcomMessages


