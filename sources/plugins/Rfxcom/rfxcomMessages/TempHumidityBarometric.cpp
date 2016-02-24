#include "stdafx.h"
#include "TempHumidityBarometric.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CTempHumidityBarometric::CTempHumidityBarometric(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_temperature("temperature"), m_humidity("humidity"), m_pressure("pressure"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeTEMP_HUM_BARO, GET_RBUF_STRUCT_SIZE(TEMP_HUM_BARO), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.TEMP_HUM_BARO.subtype;

   m_id = rbuf.TEMP_HUM_BARO.id1 | (rbuf.TEMP_HUM_BARO.id2 << 8);

   m_temperature.set(NormalizeTemperature(rbuf.TEMP_HUM_BARO.temperatureh, rbuf.TEMP_HUM_BARO.temperaturel, rbuf.TEMP_HUM_BARO.tempsign == 1));
   m_humidity.set(rbuf.TEMP_HUM_BARO.humidity);

   m_pressure.set(rbuf.TEMP_HUM_BARO.baroh << 8 | (rbuf.TEMP_HUM_BARO.barol));

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.TEMP_HUM_BARO.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.TEMP_HUM_BARO.rssi));

   Init(context);
}

CTempHumidityBarometric::~CTempHumidityBarometric()
{
}

void CTempHumidityBarometric::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeTEMP_HUM_BARO);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_temperature);
      context->declareKeyword(m_deviceName, m_humidity);
      context->declareKeyword(m_deviceName, m_pressure);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CTempHumidityBarometric::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("TempHumidityBarometric is a read-only message, can not be encoded");
}

void CTempHumidityBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_temperature);
   context->historizeData(m_deviceName, m_humidity);
   context->historizeData(m_deviceName, m_pressure);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CTempHumidityBarometric::getDeviceName() const
{
   return m_deviceName;
}                     

void CTempHumidityBarometric::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CTempHumidityBarometric::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeTHB1: ssModel << "BTHR918, BTHGN129"; break;
   case sTypeTHB2: ssModel << "BTHR918N, BTHR968"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages