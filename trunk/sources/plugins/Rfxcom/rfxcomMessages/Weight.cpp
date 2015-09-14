#include "stdafx.h"
#include "Weight.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CWeight::CWeight(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_weight("weight"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeWEIGHT, GET_RBUF_STRUCT_SIZE(WEIGHT), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.WEIGHT.subtype;

   m_id = rbuf.WEIGHT.id1 | (rbuf.WEIGHT.id2 << 8);

   m_weight.set( (rbuf.WEIGHT.weighthigh << 8 | rbuf.WEIGHT.weightlow) / 10.0 );

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.WEIGHT.filler)); // In SDK specification battery_level is at filler location
   m_rssi.set(NormalizeRssiLevel(rbuf.WEIGHT.rssi));

   Init(context);
}

CWeight::~CWeight()
{
}

void CWeight::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeWEIGHT);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_weight);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CWeight::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Weight is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Weight is a read-only message, can not be encoded");
}

void CWeight::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_weight);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CWeight::getDeviceName() const
{
   return m_deviceName;
}                     

void CWeight::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CWeight::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeWEIGHT1: ssModel << "BWR101/102"; break;
   case sTypeWEIGHT2: ssModel << "GR101"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages