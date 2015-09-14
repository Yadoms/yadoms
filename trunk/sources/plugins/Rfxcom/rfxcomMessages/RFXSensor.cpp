#include "stdafx.h"
#include "RFXSensor.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CRFXSensor::CRFXSensor(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_temperature("temperature"), m_temperatureAvailable(false), m_adVoltage("adVoltage"), m_adVoltageAvailable(false), m_voltage("voltage"), m_voltageAvailable(false), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeRFXSensor, GET_RBUF_STRUCT_SIZE(RFXSENSOR), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.RFXSENSOR.subtype;

   m_id = rbuf.RFXSENSOR.id;

   switch(rbuf.RFXSENSOR.subtype)
   {
   case sTypeRFXSensorTemp:
      {
         m_temperatureAvailable = true;
         int temperature = ((rbuf.RFXSENSOR.msg1 & 0x7F) << 8 | rbuf.RFXSENSOR.msg2);
         if (rbuf.RFXSENSOR.msg1 & 0x80)
            temperature = -temperature;
         m_temperature.set(temperature / 100);
         break;
      }
   case sTypeRFXSensorAD:
      {
         m_adVoltageAvailable = true;
         m_adVoltage.set((rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2) / 1000);
         break;
      }
   case sTypeRFXSensorVolt:
      {
         m_voltageAvailable = true;
         m_voltage.set((rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2) / 1000);
         break;
      }
   case sTypeRFXSensorMessage:
      {
         processMessage(rbuf.RFXSENSOR.msg1 << 8 | rbuf.RFXSENSOR.msg2);
         break;
      }
   default:
      {
         YADOMS_LOG(warning) << "Unsupported RFXSensor sub-type " << rbuf.RFXSENSOR.subtype;
         break;
      }
   }

   m_rssi.set(NormalizeRssiLevel(rbuf.RFXSENSOR.rssi));

   Init(context);
}

CRFXSensor::~CRFXSensor()
{
}

void CRFXSensor::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName) && (m_temperatureAvailable || m_adVoltageAvailable || m_voltageAvailable))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRFXSensor);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      if (m_temperatureAvailable)
         context->declareKeyword(m_deviceName, m_temperature);

      if (m_adVoltageAvailable)
         context->declareKeyword(m_deviceName, m_adVoltage);

      if (m_voltageAvailable)
         context->declareKeyword(m_deviceName, m_voltage);

      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRFXSensor::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "RFXSensor is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("RFXSensor is a read-only message, can not be encoded");
}

void CRFXSensor::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   if (m_temperatureAvailable || m_adVoltageAvailable || m_voltageAvailable)
   {
      if (m_temperatureAvailable)
         context->historizeData(m_deviceName, m_temperature);

      if (m_adVoltageAvailable)
         context->historizeData(m_deviceName, m_adVoltage);

      if (m_voltageAvailable)
         context->historizeData(m_deviceName, m_voltage);

      context->historizeData(m_deviceName, m_rssi);
   }
}

const std::string& CRFXSensor::getDeviceName() const
{
   return m_deviceName;
}                     

void CRFXSensor::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CRFXSensor::buildDeviceModel()
{
   m_deviceModel = "RFXSensor";
}

void CRFXSensor::processMessage(unsigned int msgId)
{
   switch(msgId)
   {
   case 0x0001: YADOMS_LOG(warning) << "RFXSensor received message \"Sensor addresses incremented\""; break;
   case 0x0002: YADOMS_LOG(warning) << "RFXSensor received message \"Battery low detected\""; break;
   case 0x0081: YADOMS_LOG(warning) << "RFXSensor received message \"No 1-wire device connected\""; break;
   case 0x0082: YADOMS_LOG(warning) << "RFXSensor received message \"1-Wire ROM CRC error\""; break;
   case 0x0083: YADOMS_LOG(warning) << "RFXSensor received message \"1-Wire device connected is not a DS18B20 or DS2438\""; break;
   case 0x0084: YADOMS_LOG(warning) << "RFXSensor received message \"No end of read signal received from 1-Wiredevice\""; break;
   case 0x0085: YADOMS_LOG(warning) << "RFXSensor received message \"1-Wire scratchpad CRC error\""; break;
   default: YADOMS_LOG(warning) << "RFXSensor invalid received message (unknown ID " << msgId << ")" ; break;
   }
}

} // namespace rfxcomMessages