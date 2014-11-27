#include "stdafx.h"
#include "Thermostat2.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYadomsApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(command);
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_unitCode = deviceDetails.get<unsigned int>("unitCode");

   Init(context);
}

CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_rssi("rssi")
{
   m_state.set(false);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeHE105 :
   case sTypeRTS10 :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned int>("unitCode");

   Init(context);
}

CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeThermostat2, GET_RBUF_STRUCT_SIZE(THERMOSTAT2), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.THERMOSTAT2.subtype;
   m_unitCode = rbuf.THERMOSTAT2.unitcode;
   m_state.set(fromProtocolState(rbuf.THERMOSTAT2.cmnd));
   m_rssi.set(NormalizeRssiLevel(rbuf.THERMOSTAT2.rssi));

   Init(context);
}

CThermostat2::~CThermostat2()
{
}

void CThermostat2::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeThermostat2);
      details.set("subType", m_subType);
      details.set("unitCode", m_unitCode);

      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CThermostat2::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.THERMOSTAT2);

   rbuf.THERMOSTAT2.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT2);
   rbuf.THERMOSTAT2.packettype = pTypeThermostat2;
   rbuf.THERMOSTAT2.subtype = m_subType;
   rbuf.THERMOSTAT2.seqnbr = seqNumberProvider->next();
   rbuf.THERMOSTAT2.unitcode = (BYTE)m_unitCode;
   rbuf.THERMOSTAT2.cmnd = toProtocolState(m_state);
   rbuf.THERMOSTAT2.rssi = 0;
   rbuf.THERMOSTAT2.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT2));
}

void CThermostat2::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CThermostat2::getDeviceName() const
{
   return m_deviceName;
}                     

void CThermostat2::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CThermostat2::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeHE105: ssModel << "HE105"; break;
   case sTypeRTS10: ssModel << "RTS10, RFS10, TLX1206"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

unsigned char CThermostat2::toProtocolState(const yApi::historization::CSwitch& switchState)
{
   return switchState.get() ? thermostat2_sOn : thermostat2_sOff;
}

bool CThermostat2::fromProtocolState(unsigned char protocolState)
{
   switch(protocolState)
   {
   case thermostat2_sOn: return true;
   case thermostat2_sOff: return false;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
   }
}

} // namespace rfxcomMessages