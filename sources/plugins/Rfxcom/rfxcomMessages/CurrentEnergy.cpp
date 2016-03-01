#include "stdafx.h"
#include "CurrentEnergy.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCurrentEnergy::CCurrentEnergy(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_current1("channel_1"), m_current2("channel_2"), m_current3("channel_3"), m_instantPower("instant"), m_totalPowerAvailable(false), m_totalPower("total"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeCURRENTENERGY, GET_RBUF_STRUCT_SIZE(CURRENT_ENERGY), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CURRENT_ENERGY.subtype;

   m_id = rbuf.CURRENT_ENERGY.id1 | (rbuf.CURRENT_ENERGY.id2 << 8);

   m_current1.set(rbuf.CURRENT.ch1h << 8 | rbuf.CURRENT.ch1l);
   m_current2.set(rbuf.CURRENT.ch2h << 8 | rbuf.CURRENT.ch2l);
   m_current3.set(rbuf.CURRENT.ch3h << 8 | rbuf.CURRENT.ch3l);

   if (rbuf.CURRENT_ENERGY.count == 0)
   {
      m_totalPowerAvailable = true;
      unsigned long totalPower = (unsigned long) (rbuf.CURRENT_ENERGY.total3 << 24 | rbuf.CURRENT_ENERGY.total4 << 16 | rbuf.CURRENT_ENERGY.total5 << 8 | rbuf.CURRENT_ENERGY.total6);
      totalPower += rbuf.CURRENT_ENERGY.total2 * 2 ^ 32;
      totalPower += rbuf.CURRENT_ENERGY.total1 * 2 ^ 40;
      m_totalPower.set((unsigned long) (totalPower / 223.666));
   }

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.CURRENT_ENERGY.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.CURRENT_ENERGY.rssi));

   Init(context);
}

CCurrentEnergy::~CCurrentEnergy()
{
}

void CCurrentEnergy::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCURRENTENERGY);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details);

      context->declareKeyword(m_deviceName, m_instantPower);
      if (m_totalPowerAvailable)
         context->declareKeyword(m_deviceName, m_totalPower);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCurrentEnergy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("CurrentEnergy is a read-only message, can not be encoded");
}

void CCurrentEnergy::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_instantPower);
   if (m_totalPowerAvailable)
      context->historizeData(m_deviceName, m_totalPower);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CCurrentEnergy::getDeviceName() const
{
   return m_deviceName;
}                     

void CCurrentEnergy::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CCurrentEnergy::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeELEC4: ssModel << "CM180i"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages