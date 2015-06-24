#include "stdafx.h"
#include "Energy.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CEnergy::CEnergy(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_instantPower("instant"), m_totalPower("total"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeENERGY, GET_RBUF_STRUCT_SIZE(ENERGY), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.ENERGY.subtype;

   m_id = rbuf.ENERGY.id1 | (rbuf.ENERGY.id2 << 8);

   m_instantPower.set(rbuf.ENERGY.instant1 << 24 | rbuf.ENERGY.instant2 << 16 | rbuf.ENERGY.instant3 << 8 | rbuf.ENERGY.instant4);

   long totalPower = (long) (rbuf.ENERGY.total3 << 24 | rbuf.ENERGY.total4 << 16 | rbuf.ENERGY.total5 << 8 | rbuf.ENERGY.total6);
   totalPower += rbuf.ENERGY.total2 * 2 ^ 32;
   totalPower += rbuf.ENERGY.total1 * 2 ^ 40;
   m_totalPower.set(static_cast<long>(totalPower / 223.666));

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.ENERGY.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.ENERGY.rssi));

   Init(context);
}

CEnergy::~CEnergy()
{
}

void CEnergy::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeENERGY);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_instantPower);
      context->declareKeyword(m_deviceName, m_totalPower);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CEnergy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Energy is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Energy is a read-only message, can not be encoded");
}

void CEnergy::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_instantPower);
   context->historizeData(m_deviceName, m_totalPower);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CEnergy::getDeviceName() const
{
   return m_deviceName;
}                     

void CEnergy::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CEnergy::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeELEC2: ssModel << "CM119/160"; break;
   case sTypeELEC3: ssModel << "CM180"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages