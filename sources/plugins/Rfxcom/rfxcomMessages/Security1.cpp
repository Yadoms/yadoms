#include "stdafx.h"
#include "Security1.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Security1X10.h"
#include "Security1X10M.h"
#include "Security1X10R.h"
#include "Security1KD101_SA30.h"
#include "Security1PowerCodeSensor.h"
#include "Security1PowerCodeMotion.h"
#include "Security1Meiantech.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_id = deviceDetails.get<unsigned int>("id");

   declare(context);
   m_subTypeManager->set(keyword, command);
}

CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(subType);
   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

   declare(context);
   m_subTypeManager->reset();
}

CSecurity1::CSecurity1(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeSecurity1, GET_RBUF_STRUCT_SIZE(SECURITY1), DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.SECURITY1.subtype);
   m_id = (rbuf.SECURITY1.id1 << 8) | rbuf.SECURITY1.id2;
   m_subTypeManager->setFromProtocolState(rbuf.SECURITY1.status);
   m_rssi.set(NormalizeRssiLevel(rbuf.SECURITY1.rssi));

   declare(context);
}

CSecurity1::~CSecurity1()
{
}

void CSecurity1::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case CSecurity1X10::rfxValue : m_subTypeManager.reset(new CSecurity1X10()); break;
   case CSecurity1X10M::rfxValue : m_subTypeManager.reset(new CSecurity1X10M()); break;
   case CSecurity1X10R::rfxValue : m_subTypeManager.reset(new CSecurity1X10R()); break;
   case CSecurity1KD101_SA30::rfxValueKD101 : m_subTypeManager.reset(new CSecurity1KD101_SA30("KD101")); break;
   case CSecurity1PowerCodeSensor::rfxValuePowercodeSensor : m_subTypeManager.reset(new CSecurity1PowerCodeSensor(false)); break;
   case CSecurity1PowerCodeSensor::rfxValuePowercodeSensorAux : m_subTypeManager.reset(new CSecurity1PowerCodeSensor(true)); break;
   case CSecurity1PowerCodeMotion::rfxValue : m_subTypeManager.reset(new CSecurity1PowerCodeMotion()); break;
   case CSecurity1Meiantech::rfxValue : m_subTypeManager.reset(new CSecurity1Meiantech()); break;
   case CSecurity1KD101_SA30::rfxValueSA30 : m_subTypeManager.reset(new CSecurity1KD101_SA30("SA30")); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CSecurity1::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   BOOST_ASSERT_MSG(!!m_subTypeManager, "m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeSecurity1);
      details.set("subType", m_subType);
      details.set("id", m_id);

      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details.serialize());

      context->declareKeyword(m_deviceName, m_rssi);
   }

   m_subTypeManager->declare(context, m_deviceName);
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CSecurity1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.SECURITY1);

   rbuf.SECURITY1.packetlength = ENCODE_PACKET_LENGTH(SECURITY1);
   rbuf.SECURITY1.packettype = pTypeSecurity1;
   rbuf.SECURITY1.subtype = m_subType;
   rbuf.SECURITY1.seqnbr = seqNumberProvider->next();
   rbuf.SECURITY1.id1 = (unsigned char)((m_id & 0xFF00) >> 8);
   rbuf.SECURITY1.id2 = (unsigned char)(m_id & 0xFF);
   rbuf.SECURITY1.status = m_subTypeManager->toProtocolState();
   rbuf.SECURITY1.rssi = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(SECURITY1));
}

void CSecurity1::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CSecurity1::getDeviceName() const
{
   return m_deviceName;
}                     

void CSecurity1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << m_id;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages