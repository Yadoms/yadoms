#include "stdafx.h"
#include "UV.h"
#include <shared/Log.h>
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CUV::CUV(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_uv("uv"), m_temperature("temperature"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeUV, GET_RBUF_STRUCT_SIZE(UV), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.UV.subtype;

   m_id = rbuf.UV.id1 | (rbuf.UV.id2 << 8);

   m_uv.set(rbuf.UV.uv / 10);
   if (m_subType == sTypeUV3)
      m_temperature.set(NormalizeTemperature(rbuf.UV.temperatureh, rbuf.UV.temperaturel, rbuf.UV.tempsign == 1));
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.UV.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.UV.rssi));

   Init(context);
}

CUV::~CUV()
{
}

void CUV::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeUV);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      if (m_subType == sTypeUV3)
         context->declareKeyword(m_deviceName, m_temperature);

      context->declareKeyword(m_deviceName, m_uv);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CUV::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "UV is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("UV is a read-only message, can not be encoded");
}

void CUV::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (m_subType == sTypeUV3)
      context->historizeData(m_deviceName, m_temperature);

   context->historizeData(m_deviceName, m_uv);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CUV::getDeviceName() const
{
   return m_deviceName;
}                     

void CUV::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CUV::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeUV1: ssModel << "UVN128, UV138"; break;
   case sTypeUV2: ssModel << "UVN800"; break;
   case sTypeUV3: ssModel << "TFA"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages