#include "stdafx.h"
#include "TempHumidity.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CTempHumidity::CTempHumidity(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_temperature("temperature"), m_humidity("humidity"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeTEMP_HUM, GET_RBUF_STRUCT_SIZE(TEMP_HUM), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.TEMP_HUM.subtype;

   m_id = rbuf.TEMP_HUM.id1 | (rbuf.TEMP_HUM.id2 << 8);

   m_temperature.set(NormalizeTemperature(rbuf.TEMP_HUM.temperatureh, rbuf.TEMP_HUM.temperaturel, rbuf.TEMP_HUM.tempsign));
   m_humidity.set(rbuf.TEMP_HUM.humidity);

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.TEMP_HUM.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.TEMP_HUM.rssi));

   Init(context);
}

CTempHumidity::~CTempHumidity()
{
}

void CTempHumidity::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeTEMP_HUM);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_temperature);
      context->declareKeyword(m_deviceName, m_humidity);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CTempHumidity::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return shared::communication::CByteBuffer();
}

void CTempHumidity::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_temperature);
   context->historizeData(m_deviceName, m_humidity);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

void CTempHumidity::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CTempHumidity::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeTH1: ssModel << "THGN122/123, THGN132, THGR122/228/238/268"; break;
   case sTypeTH2: ssModel << "THGR810, THGN800"; break;
   case sTypeTH3: ssModel << "RTGR328"; break;
   case sTypeTH4: ssModel << "THGR328"; break;
   case sTypeTH5: ssModel << "WTGR800"; break;
   case sTypeTH6: ssModel << "THGR918, THGRN228, THGN500"; break;
   case sTypeTH7: ssModel << "TFA TS34C, Cresta"; break;
   case sTypeTH8: ssModel << "WT450H"; break;
   case sTypeTH9: ssModel << "Viking 02035, 02038 (02035 has no humidity)"; break;
   case sTypeTH10: ssModel << "Rubicson"; break;
   case sTypeTH11: ssModel << "EW109"; break;
   case sTypeTH12: ssModel << "Imagintronix"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages