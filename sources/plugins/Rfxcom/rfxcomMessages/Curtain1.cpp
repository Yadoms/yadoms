#include "stdafx.h"
#include "Curtain1.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_state("state")
{
   m_state.set(command);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_houseCode = deviceDetails.get<unsigned char>("houseCode");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");

   Init(context);
}

CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_state("state"), m_subType(0), m_houseCode(0), m_unitCode(0)
{
   m_state.set(yApi::historization::ECurtainCommand::kStopValue);

   m_subType = subType;
   if (m_subType != sTypeHarrison)
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

   m_houseCode = (unsigned char) manuallyDeviceCreationConfiguration.get<char>("houseCode");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
}

CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_subType(0), m_houseCode(0), m_unitCode(0)
{
   // Should not be called (transmitter-only device)
   BOOST_ASSERT_MSG(false, "Constructing Curtain1 object from received buffer is not possible, Curtain1 is transmitter-only device");
}

CCurtain1::~CCurtain1()
{
}

void CCurtain1::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCurtain);
      details.set("subType", m_subType);
      details.set("houseCode", m_houseCode);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_state);
   }
}

boost::shared_ptr<std::queue<const shared::communication::CByteBuffer> > CCurtain1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.CURTAIN1);

   buffer.CURTAIN1.packetlength = ENCODE_PACKET_LENGTH(CURTAIN1);
   buffer.CURTAIN1.packettype = pTypeCurtain;
   buffer.CURTAIN1.subtype = m_subType;
   buffer.CURTAIN1.seqnbr = seqNumberProvider->next();
   buffer.CURTAIN1.housecode = m_houseCode;
   buffer.CURTAIN1.unitcode = m_unitCode;
   buffer.CURTAIN1.cmnd = toProtocolState(m_state);
   buffer.CURTAIN1.filler = 0;

   return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(CURTAIN1));
}

void CCurtain1::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   // Nothing to historize (transmitter-only device)
}

void CCurtain1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (char)m_houseCode << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CCurtain1::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeHarrison: ssModel << "Harrison"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

unsigned char CCurtain1::toProtocolState(const yApi::historization::CCurtain& curtainState)
{
   switch (curtainState.get()())
   {
   case yApi::historization::ECurtainCommand::kOpenValue: return curtain_sOpen;
   case yApi::historization::ECurtainCommand::kCloseValue: return curtain_sClose;
   case yApi::historization::ECurtainCommand::kStopValue: return curtain_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(curtainState.formatValue());
   }
}

} // namespace rfxcomMessages