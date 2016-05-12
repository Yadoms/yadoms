#include "stdafx.h"
#include "CurrentEnergy.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCurrentEnergy::CCurrentEnergy(boost::shared_ptr<yApi::IYPluginApi> api, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_current1("channel_1"), m_current2("channel_2"), m_current3("channel_3"), m_instantPower("instant"), m_totalPower("total"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf,
                        rbufSize,
                        pTypeCURRENTENERGY,
                        DONT_CHECK_SUBTYPE,
                        GET_RBUF_STRUCT_SIZE(CURRENT_ENERGY),
                        DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.CURRENT_ENERGY.subtype;

   m_id = rbuf.CURRENT_ENERGY.id1 | (rbuf.CURRENT_ENERGY.id2 << 8);

   m_current1.set(rbuf.CURRENT.ch1h << 8 | rbuf.CURRENT.ch1l);
   m_current2.set(rbuf.CURRENT.ch2h << 8 | rbuf.CURRENT.ch2l);
   m_current3.set(rbuf.CURRENT.ch3h << 8 | rbuf.CURRENT.ch3l);

   if (rbuf.CURRENT_ENERGY.count == 0)
   {
      Poco::Int64 totalPower = rbuf.CURRENT_ENERGY.total3 << 24 | rbuf.CURRENT_ENERGY.total4 << 16 | rbuf.CURRENT_ENERGY.total5 << 8 | rbuf.CURRENT_ENERGY.total6;
      totalPower += rbuf.CURRENT_ENERGY.total2 * 2 ^ 32;
      totalPower += rbuf.CURRENT_ENERGY.total1 * 2 ^ 40;
      m_totalPower->set(static_cast<Poco::Int64>(totalPower / 223.666));
   }
   else
   {
      // No total power on CM180 if count > 0
      m_totalPower = boost::none;
   }

   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.CURRENT_ENERGY.battery_level));
   m_rssi.set(NormalizeRssiLevel(rbuf.CURRENT_ENERGY.rssi));

   Init(api);
}

CCurrentEnergy::~CCurrentEnergy()
{
}

void CCurrentEnergy::Init(boost::shared_ptr<yApi::IYPluginApi> api)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!api->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCURRENTENERGY);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, details);

      api->declareKeyword(m_deviceName, m_instantPower);
      if (m_totalPower)
         api->declareKeyword(m_deviceName, *m_totalPower);
      api->declareKeyword(m_deviceName, m_batteryLevel);
      api->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCurrentEnergy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("CurrentEnergy is a read-only message, can not be encoded");
}

void CCurrentEnergy::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
{
   api->historizeData(m_deviceName, m_instantPower);
   if (m_totalPower)
      api->historizeData(m_deviceName, *m_totalPower);
   api->historizeData(m_deviceName, m_batteryLevel);
   api->historizeData(m_deviceName, m_rssi);
}

const std::string& CCurrentEnergy::getDeviceName() const
{
   return m_deviceName;
}                     

void CCurrentEnergy::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
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