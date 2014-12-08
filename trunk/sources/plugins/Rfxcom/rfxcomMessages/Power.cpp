#include "stdafx.h"
#include "Power.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CPower::CPower(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_voltage("voltage"), m_instantPower("instant"), m_current("current"), m_totalPower("total"), m_powerFactor("powerFactor"), m_frequency("frequency"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypePOWER, GET_RBUF_STRUCT_SIZE(POWER), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.POWER.subtype;

   m_id = rbuf.POWER.id1 | (rbuf.POWER.id2 << 8);

   m_voltage.set(rbuf.POWER.voltage);

   m_current.set( (rbuf.POWER.currentH << 8 | rbuf.POWER.currentL) / 100.0);

   m_instantPower.set( (rbuf.POWER.powerH << 8 | rbuf.POWER.powerL) / 10.0);

   m_totalPower.set( (rbuf.POWER.energyH << 8 | rbuf.POWER.energyL) / 100.0);

   m_powerFactor.set(rbuf.POWER.pf / 100.0);

   m_frequency.set(rbuf.POWER.freq);

   m_rssi.set(NormalizeRssiLevel(rbuf.POWER.rssi));

   Init(context);
}

CPower::~CPower()
{
}

void CPower::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypePOWER);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_voltage);
      context->declareKeyword(m_deviceName, m_current);
      context->declareKeyword(m_deviceName, m_instantPower);
      context->declareKeyword(m_deviceName, m_totalPower);
      context->declareKeyword(m_deviceName, m_powerFactor);
      context->declareKeyword(m_deviceName, m_frequency);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CPower::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   BOOST_ASSERT_MSG(false, "Power is a read-only message, can not be encoded");
   throw shared::exception::CInvalidParameter("Power is a read-only message, can not be encoded");
}

void CPower::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_voltage);
   context->historizeData(m_deviceName, m_current);
   context->historizeData(m_deviceName, m_instantPower);
   context->historizeData(m_deviceName, m_totalPower);
   context->historizeData(m_deviceName, m_powerFactor);
   context->historizeData(m_deviceName, m_frequency);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CPower::getDeviceName() const
{
   return m_deviceName;
}                     

void CPower::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CPower::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeELEC5: ssModel << "Revolt"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

} // namespace rfxcomMessages