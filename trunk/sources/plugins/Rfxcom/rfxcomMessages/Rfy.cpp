#include "stdafx.h"
#include "Rfy.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/plugin/yadomsApi/commands/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CRfy::CRfy(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state")
{
   m_state.set(command);

   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");
   m_unitCode = deviceParameters.get<unsigned char>("unitCode");

   Init(context);
}

CRfy::CRfy(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_subType(0), m_id(0), m_unitCode(0)
{
   // Should not be called (transmitter-only device)
   BOOST_ASSERT_MSG(false, "Constructing CRfy object from received buffer is not possible, CRfy is transmitter-only device");
}

CRfy::~CRfy()
{
}

void CRfy::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Nothing to declare (transmitter-only device)
}

const shared::communication::CByteBuffer CRfy::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.RFY);

   rbuf.RFY.packetlength = ENCODE_PACKET_LENGTH(RFY);
   rbuf.RFY.packettype = pTypeRFY;
   rbuf.RFY.subtype = m_subType;
   rbuf.RFY.seqnbr = seqNumberProvider->next();
   rbuf.RFY.id1 = (unsigned char)((m_id & 0xFF0000) >> 16);
   rbuf.RFY.id2 = (unsigned char)((m_id & 0xFF00) >> 8);
   rbuf.RFY.id3 = (unsigned char)(m_id & 0xFF);
   rbuf.RFY.unitcode = m_unitCode;
   rbuf.RFY.cmnd = toProtocolState(m_state);
   rbuf.RFY.rfu1 = 0;
   rbuf.RFY.rfu2 = 0;
   rbuf.RFY.rfu3 = 0;
   rbuf.RFY.rssi = 0;
   rbuf.LIGHTING1.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(RFY));
}

void CRfy::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   // Nothing to historize (transmitter-only device)
}

void CRfy::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CRfy::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeRFY: ssModel << "Somfy"; break;
   case sTypeRFYext: ssModel << "SomfyExt"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

unsigned char CRfy::toProtocolState(const yApi::commands::CCurtain& curtainState)
{
   switch (curtainState.command()())
   {
   case yApi::commands::CCurtain::ECommand::kOpen: return rfy_sUp;
   case yApi::commands::CCurtain::ECommand::kClose: return rfy_sDown;
   case yApi::commands::CCurtain::ECommand::kStop: return rfy_sStop;
   default:
      BOOST_ASSERT_MSG(false, "Unsupported value");
      throw shared::exception::CInvalidParameter(curtainState.formatValue());
   }
}

} // namespace rfxcomMessages