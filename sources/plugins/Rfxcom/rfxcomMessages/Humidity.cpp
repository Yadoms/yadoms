#include "stdafx.h"
#include "Humidity.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CHumidity::CHumidity(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_humidity("humidity"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeHUM, GET_RBUF_STRUCT_SIZE(HUM), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.HUM.subtype;

   m_id = rbuf.HUM.id1 | (rbuf.HUM.id2 << 8);

   m_humidity.set(rbuf.HUM.humidity);

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.HUM.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.HUM.rssi));

   Init(context);
}

CHumidity::~CHumidity()
{
}

void CHumidity::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeHUM);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_humidity);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CHumidity::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Humidity is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Humidity is a read-only message, can not be encoded");
}

void CHumidity::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_humidity);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CHumidity::getDeviceName() const
{
   return m_deviceName;
}                     

void CHumidity::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CHumidity::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeHUM1: ssModel << "La Crosse TX3"; break;
   case sTypeHUM2: ssModel << "La Crosse WS2300"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages