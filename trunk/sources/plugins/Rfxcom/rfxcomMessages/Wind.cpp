#include "stdafx.h"
#include "Wind.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CWind::CWind(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_windDirection("windDirection"), m_windAverageSpeed("windAverageSpeed"), m_windMaxSpeed("windMaxSpeed"), m_temperature("temperature"),
   m_chillTemperature("chillTemperature"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeWIND, GET_RBUF_STRUCT_SIZE(WIND), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.WIND.subtype;

   m_id = rbuf.WIND.id1 | (rbuf.WIND.id2 << 8);

   m_windDirection.set(rbuf.WIND.directionl | (rbuf.WIND.directionh << 8));
   if (m_subType != sTypeWIND5)
      m_windAverageSpeed.set( (rbuf.WIND.av_speedl | (rbuf.WIND.av_speedh << 8)) / 10.0);
   m_windMaxSpeed.set( (rbuf.WIND.gustl | (rbuf.WIND.gusth << 8)) / 10.0);

   if (m_subType == sTypeWIND4)
   {
      m_temperature.set(NormalizeTemperature(rbuf.WIND.temperatureh, rbuf.WIND.temperaturel, rbuf.WIND.tempsign == 1));
      m_chillTemperature.set(NormalizeTemperature(rbuf.WIND.chillh, rbuf.WIND.chilll, rbuf.WIND.chillsign == 1));
   }

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.WIND.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.WIND.rssi));

   Init(context);
}

CWind::~CWind()
{
}

void CWind::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeWIND);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_windDirection);
      context->declareKeyword(m_deviceName, m_windMaxSpeed);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);

      if (m_subType != sTypeWIND5)
         context->declareKeyword(m_deviceName, m_windAverageSpeed);

      if (m_subType == sTypeWIND4)
      {
         context->declareKeyword(m_deviceName, m_temperature);
         context->declareKeyword(m_deviceName, m_chillTemperature);
      }
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CWind::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Wind is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Wind is a read-only message, can not be encoded");
}

void CWind::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_windDirection);
   context->historizeData(m_deviceName, m_windMaxSpeed);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);

   if (m_subType != sTypeWIND5)
      context->historizeData(m_deviceName, m_windAverageSpeed);

   if (m_subType == sTypeWIND4)
   {
      context->historizeData(m_deviceName, m_temperature);
      context->historizeData(m_deviceName, m_chillTemperature);
   }
}

const std::string& CWind::getDeviceName() const
{
   return m_deviceName;
}                     

void CWind::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CWind::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeWIND1: ssModel << "WTGR800"; break;
   case sTypeWIND2: ssModel << "WGR800"; break;
   case sTypeWIND3: ssModel << "STR918,WGR918"; break;
   case sTypeWIND4: ssModel << "TFA"; break;
   case sTypeWIND5: ssModel << "UPM"; break;
   case sTypeWIND6: ssModel << "WS2300"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages