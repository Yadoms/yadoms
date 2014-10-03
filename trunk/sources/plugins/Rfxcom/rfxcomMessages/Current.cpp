#include "stdafx.h"
#include "Current.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurrent::CCurrent(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_current1("channel_1"), m_current2("channel_2"), m_current3("channel_3"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeCURRENT, GET_RBUF_STRUCT_SIZE(CURRENT), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CURRENT.subtype;

   m_id = rbuf.CURRENT.id1 | (rbuf.CURRENT.id2 << 8);

   m_current1.set(rbuf.CURRENT.ch1h << 8 | rbuf.CURRENT.ch1l);
   m_current2.set(rbuf.CURRENT.ch2h << 8 | rbuf.CURRENT.ch2l);
   m_current3.set(rbuf.CURRENT.ch3h << 8 | rbuf.CURRENT.ch3l);

   m_batteryLevel.set(rbuf.CURRENT.battery_level == 0x09 ? 100 : 0);

   m_rssi.set(rbuf.CURRENT.rssi * 100 / 0x0F);

   Init(context);
}

CCurrent::~CCurrent()
{
}

void CCurrent::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCURRENT);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_current1);
      context->declareKeyword(m_deviceName, m_current2);
      context->declareKeyword(m_deviceName, m_current3);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CCurrent::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return shared::communication::CByteBuffer();
}

void CCurrent::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_current1);
   context->historizeData(m_deviceName, m_current2);
   context->historizeData(m_deviceName, m_current3);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

void CCurrent::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CCurrent::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeELEC1: ssModel << "CM113, Electrisave"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages