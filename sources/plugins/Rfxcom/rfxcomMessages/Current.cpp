#include "stdafx.h"
#include "Current.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCurrent::CCurrent(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_current1("channel_1"), m_current2("channel_2"), m_current3("channel_3"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf,
                        rbufSize,
                        pTypeCURRENT,
                        DONT_CHECK_SUBTYPE,
                        GET_RBUF_STRUCT_SIZE(CURRENT),
                        DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CURRENT.subtype;

   m_id = rbuf.CURRENT.id1 | (rbuf.CURRENT.id2 << 8);

   m_current1.set(rbuf.CURRENT.ch1h << 8 | rbuf.CURRENT.ch1l);
   m_current2.set(rbuf.CURRENT.ch2h << 8 | rbuf.CURRENT.ch2l);
   m_current3.set(rbuf.CURRENT.ch3h << 8 | rbuf.CURRENT.ch3l);

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.CURRENT.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.CURRENT.rssi));

   Init(context);
}

CCurrent::~CCurrent()
{
}

void CCurrent::Init(boost::shared_ptr<yApi::IYPluginApi> context)
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
      context->declareDevice(m_deviceName, m_deviceModel, details);

      context->declareKeyword(m_deviceName, m_current1);
      context->declareKeyword(m_deviceName, m_current2);
      context->declareKeyword(m_deviceName, m_current3);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCurrent::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("Current is a read-only message, can not be encoded");
}

void CCurrent::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_current1);
   context->historizeData(m_deviceName, m_current2);
   context->historizeData(m_deviceName, m_current3);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CCurrent::getDeviceName() const
{
   return m_deviceName;
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