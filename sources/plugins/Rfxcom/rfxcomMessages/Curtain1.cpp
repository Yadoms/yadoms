#include "stdafx.h"
#include "Curtain1.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state")
{
   m_state.set(command);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_houseCode = deviceParameters.get<unsigned char>("houseCode");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");

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

   // Nothing to declare (transmitter-only device)
}

const CByteBuffer CCurtain1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
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

   return CByteBuffer((BYTE*)&buffer, GET_RBUF_STRUCT_SIZE(CURTAIN1));
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

unsigned char CCurtain1::toProtocolState(const yApi::commands::CCurtain& curtainState)
{
   switch (curtainState.command()())
   {
   case yApi::commands::CCurtain::ECommand::kOpen: return curtain_sOpen;
   case yApi::commands::CCurtain::ECommand::kClose: return curtain_sClose;
   case yApi::commands::CCurtain::ECommand::kStop: return curtain_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(curtainState.formatValue());
   }
}

} // namespace rfxcomMessages