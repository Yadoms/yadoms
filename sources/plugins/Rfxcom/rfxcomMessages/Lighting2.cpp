#include "stdafx.h"
#include "Lighting2.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Lighting2Dimmable.h"
#include "Lighting2OnOff.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_houseCode = deviceDetails.get<unsigned char>("houseCode");
   m_id = deviceDetails.get<unsigned int>("id");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");

   declare(context);
   m_subTypeManager->set(command);
}

CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(subType);

   m_houseCode = manuallyDeviceCreationConfiguration.get<unsigned char>("houseCode", 0);
   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   declare(context);
   m_subTypeManager->reset();
}

CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf,
                        rbufSize,
                        pTypeLighting2,
                        DONT_CHECK_SUBTYPE,
                        GET_RBUF_STRUCT_SIZE(LIGHTING2),
                        DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.LIGHTING2.subtype);
   m_subTypeManager->idFromProtocol(rbuf.LIGHTING2.id1, rbuf.LIGHTING2.id2, rbuf.LIGHTING2.id3, rbuf.LIGHTING2.id4, m_houseCode, m_id);
   m_subTypeManager->setFromProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);
   m_unitCode = rbuf.LIGHTING2.unitcode;
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING2.rssi));

   declare(context);
}

CLighting2::~CLighting2()
{
}

void CLighting2::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case sTypeAC : m_subTypeManager.reset(new CLighting2Dimmable("AC")); break;
   case sTypeHEU : m_subTypeManager.reset(new CLighting2Dimmable("HomeEasy EU")); break;
   case sTypeANSLUT : m_subTypeManager.reset(new CLighting2Dimmable("ANSLUT")); break;
   case sTypeKambrook : m_subTypeManager.reset(new CLighting2OnOff("Kambrook RF3672 (Australia)")); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CLighting2::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!m_subTypeManager)
      throw shared::exception::CException("m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting2);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details);

      m_subTypeManager->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CLighting2::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING2);

   rbuf.LIGHTING2.packetlength = ENCODE_PACKET_LENGTH(LIGHTING2);
   rbuf.LIGHTING2.packettype = pTypeLighting2;
   rbuf.LIGHTING2.subtype = m_subType;
   rbuf.LIGHTING2.seqnbr = seqNumberProvider->next();
   m_subTypeManager->idToProtocol(m_houseCode, m_id, rbuf.LIGHTING2.id1, rbuf.LIGHTING2.id2, rbuf.LIGHTING2.id3, rbuf.LIGHTING2.id4);
   m_subTypeManager->toProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);
   rbuf.LIGHTING2.unitcode = m_unitCode;
   rbuf.LIGHTING2.rssi = 0;
   rbuf.LIGHTING2.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING2));
}

void CLighting2::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CLighting2::getDeviceName() const
{
   return m_deviceName;
}                     

void CLighting2::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_houseCode << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages