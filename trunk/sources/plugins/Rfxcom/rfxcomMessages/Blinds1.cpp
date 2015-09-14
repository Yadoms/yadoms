#include "stdafx.h"
#include "Blinds1.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_state("state"), m_batteryLevel("battery"), m_rssi("rssi")
{
   m_state.set(command);
   m_batteryLevel.set(100);
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");

   Init(context);
}

CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_batteryLevel("battery"), m_rssi("rssi")
{
   m_state.set(yApi::historization::ECurtainCommand::kStopValue);
   m_batteryLevel.set(100);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeBlindsT0 :
   case sTypeBlindsT1 :
   case sTypeBlindsT2 :
   case sTypeBlindsT3 :
   case sTypeBlindsT4 :
   case sTypeBlindsT5 :
   case sTypeBlindsT6 :
   case sTypeBlindsT7 :
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode", 0);

   Init(context);
}

CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_batteryLevel("battery"), m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeBlinds, GET_RBUF_STRUCT_SIZE(BLINDS1), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.BLINDS1.subtype;
   m_id = rbuf.BLINDS1.id1 << 24 | rbuf.BLINDS1.id2 << 16 | rbuf.BLINDS1.id3 << 8 | rbuf.BLINDS1.id4;
   m_unitCode = rbuf.BLINDS1.unitcode;
   m_state.set(fromProtocolState(rbuf.BLINDS1.cmnd));
   m_batteryLevel.set(NormalizeBatteryLevel(rbuf.BLINDS1.filler));   // filler is specified as battery level in RFXtrx SDF.pdf
   m_rssi.set(NormalizeRssiLevel(rbuf.BLINDS1.rssi));

   Init(context);
}

CBlinds1::~CBlinds1()
{
}

void CBlinds1::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeBlinds);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);

      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
      context->declareKeyword(m_deviceName, m_batteryLevel);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CBlinds1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.BLINDS1);

   buffer.BLINDS1.packetlength = ENCODE_PACKET_LENGTH(BLINDS1);
   buffer.BLINDS1.packettype = pTypeBlinds;
   buffer.BLINDS1.subtype = m_subType;
   buffer.BLINDS1.seqnbr = seqNumberProvider->next();
   buffer.BLINDS1.id1 = (unsigned char) (0xFF & (m_id >> 24));
   buffer.BLINDS1.id2 = (unsigned char) (0xFF & (m_id >> 16));
   buffer.BLINDS1.id3 = (unsigned char) (0xFF & (m_id >> 8));
   buffer.BLINDS1.id4 = (unsigned char) (0xFF & m_id);
   buffer.BLINDS1.unitcode = m_unitCode;
   buffer.BLINDS1.cmnd = toProtocolState(m_state);
   buffer.BLINDS1.rssi = 0;
   buffer.BLINDS1.filler = 0;

   return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(BLINDS1));
}

void CBlinds1::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   context->historizeData(m_deviceName, m_state);
   context->historizeData(m_deviceName, m_batteryLevel);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CBlinds1::getDeviceName() const
{
   return m_deviceName;
}                     

void CBlinds1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CBlinds1::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeBlindsT0: ssModel << "RollerTrol, Hasta new"; break;
   case sTypeBlindsT1: ssModel << "Hasta old"; break;
   case sTypeBlindsT2: ssModel << "A-OK RF01"; break;
   case sTypeBlindsT3: ssModel << "A-OK AC114/AC123"; break;
   case sTypeBlindsT4: ssModel << "RAEX YR1326"; break;
   case sTypeBlindsT5: ssModel << "Media Mount"; break;
   case sTypeBlindsT6: ssModel << "DC106, YOOHA, Rohrmotor24 RMF"; break;
   case sTypeBlindsT7: ssModel << "Forest"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

yApi::historization::ECurtainCommand CBlinds1::fromProtocolState(unsigned char protocolCmnd)
{
   switch(protocolCmnd)
   {
   case blinds_sOpen: return yApi::historization::ECurtainCommand::kOpen;
   case blinds_sClose: return yApi::historization::ECurtainCommand::kClose;
   case blinds_sStop: return yApi::historization::ECurtainCommand::kStop;
   default:
      BOOST_ASSERT_MSG(false, "Invalid state");
      throw shared::exception::CInvalidParameter("state");
   }
}

unsigned char CBlinds1::toProtocolState(const yApi::historization::CCurtain& curtainState)
{
   switch (curtainState.get()())
   {
   case yApi::historization::ECurtainCommand::kOpenValue: return blinds_sOpen;
   case yApi::historization::ECurtainCommand::kCloseValue: return blinds_sClose;
   case yApi::historization::ECurtainCommand::kStopValue: return blinds_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(curtainState.formatValue());
   }
}

} // namespace rfxcomMessages