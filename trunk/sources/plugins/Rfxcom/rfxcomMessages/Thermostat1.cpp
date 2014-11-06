#include "stdafx.h"
#include "Thermostat1.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Thermostat1Digimax.h"
#include "Thermostat1DigimaxShort.h"
// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");

   Init(context);
   m_subTypeManager->set(command);
}

CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = subType;

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

   Init(context);
   m_subTypeManager->default();
}

CThermostat1::CThermostat1(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeThermostat1, GET_RBUF_STRUCT_SIZE(THERMOSTAT1), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.THERMOSTAT1.subtype;
   m_id = rbuf.THERMOSTAT1.id1 << 8 | rbuf.THERMOSTAT1.id2;
   m_subTypeManager->setFromProtocolState(rbuf);
   m_rssi.set(NormalizeRssiLevel(rbuf.THERMOSTAT1.rssi));

   Init(context);
}

CThermostat1::~CThermostat1()
{
}

void CThermostat1::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   switch(m_subType)
   {
   case sTypeAC : m_subTypeManager.reset(new CThermostat1Digimax()); break;
   case sTypeHEU : m_subTypeManager.reset(new CThermostat1DigimaxShort()); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeThermostat1);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      m_subTypeManager->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CThermostat1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.THERMOSTAT1);

   rbuf.THERMOSTAT1.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT1);
   rbuf.THERMOSTAT1.packettype = pTypeThermostat1;
   rbuf.THERMOSTAT1.subtype = m_subType;
   rbuf.THERMOSTAT1.seqnbr = seqNumberProvider->next();
   rbuf.THERMOSTAT1.id1 = (unsigned char) (0xFF & (m_id >> 8));
   rbuf.THERMOSTAT1.id2 = (unsigned char) (0xFF & m_id);
   m_subTypeManager->toProtocolState(rbuf);
   rbuf.THERMOSTAT1.rssi = 0;
   rbuf.THERMOSTAT1.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT1));
}

void CThermostat1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CThermostat1::getDeviceName() const
{
   return m_deviceName;
}                     

void CThermostat1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages