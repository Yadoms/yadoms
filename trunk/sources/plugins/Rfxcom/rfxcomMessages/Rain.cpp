#include "stdafx.h"
#include "Rain.h"
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CRain::CRain(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeRAIN, GET_RBUF_STRUCT_SIZE(RAIN), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.RAIN.subtype;

   m_id = rbuf.RAIN.id1 | (rbuf.RAIN.id2 << 8);

   m_rain.reset(new yApi::historization::CRain("rain", (m_subType == sTypeRAIN6) ? (yApi::historization::EMeasureType::kIncrement) : (yApi::historization::EMeasureType::kCumulative)));

   switch(m_subType)
   {
   case sTypeRAIN1:
   case sTypeRAIN2:
   case sTypeRAIN3:
   case sTypeRAIN4:
   case sTypeRAIN5:
      m_rain->set(((rbuf.RAIN.raintotal1 << 16) | (rbuf.RAIN.raintotal2 << 8) | rbuf.RAIN.raintotal3) / 10.0);
      break;
   case sTypeRAIN6:
      m_rain->set(rbuf.RAIN.raintotal3 * 0.266);
      break;
   default:
      YADOMS_LOG(warning) << "Rain subtype is not supported : " << m_subType;
      break;
   }

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.RAIN.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.RAIN.rssi));

   Init(context);
}

CRain::~CRain()
{
}

void CRain::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRAIN);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, *m_rain);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRain::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Rain is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Rain is a read-only message, can not be encoded");
}

void CRain::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, *m_rain);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CRain::getDeviceName() const
{
   return m_deviceName;
}                     

void CRain::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CRain::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeRAIN1: ssModel << "RGR126/682/918"; break;
   case sTypeRAIN2: ssModel << "PCR800"; break;
   case sTypeRAIN3: ssModel << "TFA"; break;
   case sTypeRAIN4: ssModel << "UPM"; break;
   case sTypeRAIN5: ssModel << "La Crosse WS2300"; break;
   case sTypeRAIN6: ssModel << "TX5"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages