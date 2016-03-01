#include "stdafx.h"
#include "Bbq.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CBbq::CBbq(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_foodTemperature("food"), m_bbqTemperature("bbq"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeBBQ, GET_RBUF_STRUCT_SIZE(BBQ), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.BBQ.subtype;

   m_id = rbuf.BBQ.id1 | (rbuf.BBQ.id2 << 8);

   // Don't use NormalizeTemperature, as temperature is given in °C, not in °C/10
   m_foodTemperature.set((double)((rbuf.BBQ.sensor1h << 8) | rbuf.BBQ.sensor1l));
   m_bbqTemperature.set((double)((rbuf.BBQ.sensor2h << 8) | rbuf.BBQ.sensor2l));
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.BBQ.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.BBQ.rssi));

   Init(context);
}

CBbq::~CBbq()
{
}

void CBbq::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeBBQ);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_foodTemperature);
      context->declareKeyword(m_deviceName, m_bbqTemperature);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CBbq::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
}

void CBbq::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_foodTemperature);
   context->historizeData(m_deviceName, m_bbqTemperature);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CBbq::getDeviceName() const
{
   return m_deviceName;
}                     

void CBbq::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << m_id;
   m_deviceName = ssdeviceName.str();
}

void CBbq::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeBBQ1: ssModel << "Maverick ET-732"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages