#include "stdafx.h"
#include "Barometric.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CBarometric::CBarometric(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_pressure("pressure"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeBARO, GET_RBUF_STRUCT_SIZE(BARO), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.BARO.subtype;

   m_id = rbuf.BARO.id1 | (rbuf.BARO.id2 << 8);

   m_pressure.set(rbuf.BARO.baro1 << 8 | (rbuf.BARO.baro2));

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.BARO.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.BARO.rssi));

   Init(context);
}

CBarometric::~CBarometric()
{
}

void CBarometric::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeBARO);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_pressure);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CBarometric::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return shared::communication::CByteBuffer();
}

void CBarometric::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_pressure);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

void CBarometric::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CBarometric::buildDeviceModel()
{
   m_deviceModel = "Barometric sensor";
}

} // namespace rfxcomMessages