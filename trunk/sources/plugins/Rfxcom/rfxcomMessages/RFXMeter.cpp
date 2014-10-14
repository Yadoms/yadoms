#include "stdafx.h"
#include "RFXMeter.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRFXMeter::CRFXMeter(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_counter("counter"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeRFXMeter, GET_RBUF_STRUCT_SIZE(RFXMETER), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.RFXMETER.subtype;

   m_id = rbuf.RFXMETER.id2 | (rbuf.RFXMETER.id1 << 8);

   m_counter.set(rbuf.RFXMETER.count4 | (rbuf.RFXMETER.count3 << 8) | (rbuf.RFXMETER.count2 << 16) | (rbuf.RFXMETER.count1 << 24));

   m_rssi.set(NormalizeRssiLevel(rbuf.RFXMETER.rssi));

   Init(context);
}

CRFXMeter::~CRFXMeter()
{
}

void CRFXMeter::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRFXMeter);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_counter);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CRFXMeter::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return shared::communication::CByteBuffer();
}

void CRFXMeter::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   if (m_subType != sTypeRFXMeterCount)
   {
      YADOMS_LOG(warning) << "RFXMeter subtype " << m_subType << " actually not supported. Please report to Yadoms development team if needed";
      return;
   }
   context->historizeData(m_deviceName, m_counter);
   context->historizeData(m_deviceName, m_rssi);
}

void CRFXMeter::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CRFXMeter::buildDeviceModel()
{
   // RFXMeter subtype value doesn't really represent a device subtype.
   // So don't use it for model name.
   m_deviceModel = "RFXMeter counter";
}

} // namespace rfxcomMessages