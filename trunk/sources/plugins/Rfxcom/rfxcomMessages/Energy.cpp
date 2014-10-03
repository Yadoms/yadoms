#include "stdafx.h"
#include "Energy.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CEnergy::CEnergy(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_instantPower("instant"), m_totalPower("total"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeENERGY, GET_RBUF_STRUCT_SIZE(ENERGY), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.ENERGY.subtype;

   m_id = rbuf.ENERGY.id1 | (rbuf.ENERGY.id2 << 8);

   m_instantPower.set(rbuf.ENERGY.instant1 << 24 | rbuf.ENERGY.instant2 << 16 | rbuf.ENERGY.instant3 << 8 | rbuf.ENERGY.instant4);

   double totalPower = (double) (rbuf.ENERGY.total3 << 24 | rbuf.ENERGY.total4 << 16 | rbuf.ENERGY.total5 << 8 | rbuf.ENERGY.total6);
   totalPower += rbuf.ENERGY.total2 * 2 ^ 32;
   totalPower += rbuf.ENERGY.total1 * 2 ^ 40;
   m_totalPower.set(totalPower / 223.666);

   m_batteryLevel.set(rbuf.ENERGY.battery_level == 0x09 ? 100 : 0);

   m_rssi.set(rbuf.ENERGY.rssi * 100 / 0x0F);

   Init(context);
}

CEnergy::~CEnergy()
{
}

void CEnergy::Init(boost::shared_ptr<yApi::IYadomsApi> context)
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

const shared::communication::CByteBuffer CEnergy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   // Nothing to do (message type is read-only)
   return shared::communication::CByteBuffer();
}

void CEnergy::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_instantPower);
   context->historizeData(m_deviceName, m_totalPower);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
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