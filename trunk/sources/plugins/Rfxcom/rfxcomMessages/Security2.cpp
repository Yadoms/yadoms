#include "stdafx.h"
#include "Security2.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Security2ClassicKeyLoq.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_batteryLevel("battery"), m_rssi("rssi")
{
   m_batteryLevel.set(100);
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_subTypeManager->setId(deviceDetails.get<unsigned int>("id"));

   declare(context);
   m_subTypeManager->set(keyword, command);
}

CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_batteryLevel("battery"), m_rssi("rssi")
{
   m_batteryLevel.set(100);
   m_rssi.set(0);

   createSubType(subType);
   m_subTypeManager->setId(manuallyDeviceCreationConfiguration.get<unsigned int>("id"));

   declare(context);
   m_subTypeManager->resetState();
}

CSecurity2::CSecurity2(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeSecurity2, GET_RBUF_STRUCT_SIZE(SECURITY2), DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.SECURITY2.subtype);
   m_subTypeManager->setFromProtocolState(rbuf);
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.SECURITY2.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.SECURITY2.rssi));

   declare(context);
}

CSecurity2::~CSecurity2()
{
}

void CSecurity2::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case CSecurity2ClassicKeyLoq::rfxValue: m_subTypeManager.reset(new CSecurity2ClassicKeyLoq()); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CSecurity2::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   BOOST_ASSERT_MSG(!!m_subTypeManager, "m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeSecurity2);
      details.set("subType", m_subType);
      details.set("id", m_subTypeManager->getId());

      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }

   m_subTypeManager->declare(context, m_deviceName);
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CSecurity2::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.SECURITY2);

   rbuf.SECURITY2.packetlength = ENCODE_PACKET_LENGTH(SECURITY2);
   rbuf.SECURITY2.packettype = pTypeSecurity2;
   rbuf.SECURITY2.subtype = m_subType;
   rbuf.SECURITY2.seqnbr = seqNumberProvider->next();
   m_subTypeManager->toProtocolState(rbuf);
   rbuf.SECURITY2.rssi = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(SECURITY2));
}

void CSecurity2::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CSecurity2::getDeviceName() const
{
   return m_deviceName;
}                     

void CSecurity2::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_subTypeManager->getId();
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages