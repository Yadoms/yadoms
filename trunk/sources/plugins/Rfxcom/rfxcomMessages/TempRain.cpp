#include "stdafx.h"
#include "TempRain.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CTempRain::CTempRain(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_temperature("temperature"), m_rain("rain", yApi::historization::EMeasureType::kCumulative), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeTEMP_RAIN, GET_RBUF_STRUCT_SIZE(TEMP_RAIN), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.TEMP_RAIN.subtype;

   m_id = rbuf.TEMP_RAIN.id1 | (rbuf.TEMP_RAIN.id2 << 8);

   m_temperature.set(NormalizeTemperature(rbuf.TEMP_RAIN.temperatureh, rbuf.TEMP_RAIN.temperaturel, rbuf.TEMP_RAIN.tempsign == 1));
   m_rain.set((double)((rbuf.TEMP_RAIN.raintotal1 << 8) | rbuf.TEMP_RAIN.raintotal2) / 10.0);
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.TEMP_RAIN.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.TEMP_RAIN.rssi));

   Init(context);
}

CTempRain::~CTempRain()
{
}

void CTempRain::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeTEMP_RAIN);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_temperature);
      context->declareKeyword(m_deviceName, m_rain);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CTempRain::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Temp is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
}

void CTempRain::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_temperature);
   context->historizeData(m_deviceName, m_rain);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

void CTempRain::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CTempRain::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeTR1: ssModel << "Alecto WS1200"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages