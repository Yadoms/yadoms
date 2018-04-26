#include "stdafx.h"
#include "RFXSensor.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRFXSensor::CRFXSensor(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
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
         YADOMS_LOG(information) << "Unsupported RFXSensor sub-type " << rbuf.RFXSENSOR.subtype ;
         break;
      }
      }

      m_signalPower->set(NormalizesignalPowerLevel(rbuf.RFXSENSOR.rssi));

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
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRFXSensor::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("RFXSensor is a read-only message, can not be encoded");
   }

   void CRFXSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CRFXSensor::filter() const
   {
   }

   void CRFXSensor::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CRFXSensor::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRFXSensor::keywords()
   {
      return m_keywords;
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
      case 0x0001: YADOMS_LOG(information) << "RFXSensor received message \"Sensor addresses incremented\"" ;
         break;
      case 0x0002: YADOMS_LOG(information) << "RFXSensor received message \"Battery low detected\"" ;
         break;
      case 0x0081: YADOMS_LOG(information) << "RFXSensor received message \"No 1-wire device connected\"" ;
         break;
      case 0x0082: YADOMS_LOG(information) << "RFXSensor received message \"1-Wire ROM CRC error\"" ;
         break;
      case 0x0083: YADOMS_LOG(information) << "RFXSensor received message \"1-Wire device connected is not a DS18B20 or DS2438\"" ;
         break;
      case 0x0084: YADOMS_LOG(information) << "RFXSensor received message \"No end of read signal received from 1-Wiredevice\"" ;
         break;
      case 0x0085: YADOMS_LOG(information) << "RFXSensor received message \"1-Wire scratchpad CRC error\"" ;
         break;
      default: YADOMS_LOG(information) << "RFXSensor invalid received message (unknown ID " << msgId << ")" ;
         break;
      }
   }
} // namespace rfxcomMessages


