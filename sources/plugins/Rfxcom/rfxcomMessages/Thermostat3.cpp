#include "stdafx.h"
#include "Thermostat3.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Thermostat3MertikG6RH4T1.h"
#include "Thermostat3MertikG6RH4TB.h"
// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_unitCode = deviceDetails.get<unsigned int>("unitCode");

   Init(context);
   m_subTypeManager->set(keyword, command);
}

CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = subType;

   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned int>("unitCode");

   Init(context);
   m_subTypeManager->reset();
}

CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeThermostat3, GET_RBUF_STRUCT_SIZE(THERMOSTAT3), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.THERMOSTAT3.subtype;
   m_unitCode = rbuf.THERMOSTAT3.unitcode1 << 16 | rbuf.THERMOSTAT3.unitcode2 << 8 | rbuf.THERMOSTAT3.unitcode3;
   m_subTypeManager->setFromProtocolState(rbuf.THERMOSTAT3.cmnd);
   m_rssi.set(NormalizeRssiLevel(rbuf.THERMOSTAT3.rssi));

   Init(context);
}

CThermostat3::~CThermostat3()
{
}

void CThermostat3::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   switch(m_subType)
   {
   case sTypeMertikG6RH4T1 : m_subTypeManager.reset(new CThermostat3MertikG6RH4T1()); break;
   case sTypeMertikG6RH4TB : m_subTypeManager.reset(new CThermostat3MertikG6RH4TB()); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeThermostat3);
      details.set("subType", m_subType);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      m_subTypeManager->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CThermostat3::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.THERMOSTAT3);

   rbuf.THERMOSTAT3.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT3);
   rbuf.THERMOSTAT3.packettype = pTypeThermostat3;
   rbuf.THERMOSTAT3.subtype = m_subType;
   rbuf.THERMOSTAT3.seqnbr = seqNumberProvider->next();
   rbuf.THERMOSTAT3.unitcode1 = (unsigned char) (0xFF & (m_unitCode >> 16));
   rbuf.THERMOSTAT3.unitcode2 = (unsigned char) (0xFF & (m_unitCode >> 8));
   rbuf.THERMOSTAT3.unitcode3 = (unsigned char) (0xFF & m_unitCode);
   m_subTypeManager->toProtocolState(rbuf.THERMOSTAT3.cmnd);
   rbuf.THERMOSTAT3.rssi = 0;
   rbuf.THERMOSTAT3.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT3));
}

void CThermostat3::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CThermostat3::getDeviceName() const
{
   return m_deviceName;
}                     

void CThermostat3::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << m_unitCode;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages