#include "stdafx.h"
#include "DateTime.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CDateTime::CDateTime(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_dateTime("datetime"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeDT, GET_RBUF_STRUCT_SIZE(DT), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.DT.subtype;

   m_id = rbuf.DT.id1 | (rbuf.DT.id2 << 8);

   m_dateTime.set(boost::posix_time::ptime(boost::gregorian::date(2000 + rbuf.DT.yy, rbuf.DT.mm, rbuf.DT.dd), 
      boost::posix_time::time_duration(rbuf.DT.hr, rbuf.DT.min, rbuf.DT.sec)));

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.DT.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.DT.rssi));

   Init(context);
}

CDateTime::~CDateTime()
{
}

void CDateTime::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeDT);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details);

      context->declareKeyword(m_deviceName, m_dateTime);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CDateTime::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
}

void CDateTime::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_dateTime);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CDateTime::getDeviceName() const
{
   return m_deviceName;
}                     

void CDateTime::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CDateTime::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeDT1: ssModel << "RTGR328N"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages