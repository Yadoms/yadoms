#include "stdafx.h"
#include "Barometric.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CBarometric::CBarometric(boost::shared_ptr<yApi::IYPluginApi> api, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_pressure("pressure"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf,
                        rbufSize,
                        pTypeBARO,
                        DONT_CHECK_SUBTYPE,
                        GET_RBUF_STRUCT_SIZE(BARO),
                        DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.BARO.subtype;

   m_id = rbuf.BARO.id1 | (rbuf.BARO.id2 << 8);

   m_pressure.set(rbuf.BARO.baro1 << 8 | (rbuf.BARO.baro2));

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.BARO.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.BARO.rssi));

   Init(api);
}

CBarometric::~CBarometric()
{
}

void CBarometric::Init(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!api->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeBARO);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, details);

      api->declareKeyword(m_deviceName, m_pressure);
      api->declareKeyword(m_deviceName, m_batteryLevel);
      api->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CBarometric::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("Barometric is a read-only message, can not be encoded");
}

void CBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   api->historizeData(m_deviceName, m_pressure);
   api->historizeData(m_deviceName, m_batteryLevel);
   api->historizeData(m_deviceName, m_rssi);
}

const std::string& CBarometric::getDeviceName() const
{
   return m_deviceName;
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