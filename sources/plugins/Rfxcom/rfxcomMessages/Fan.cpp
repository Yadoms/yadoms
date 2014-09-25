#include "stdafx.h"
#include "Fan.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CFan::CFan(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters)
   :m_state("state")
{
   m_state.set(command);

   m_lightCmd = deviceParameters.get<bool>("light");
   m_subType = deviceParameters.get<unsigned char>("subType");
   m_id = deviceParameters.get<unsigned int>("id");

   Init(context);
}

CFan::CFan(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_state("state"), m_lightCmd(false), m_subType(0), m_id(0)
{
   // Should not be called (transmitter-only device)
   BOOST_ASSERT_MSG(false, "Constructing CFan object from received buffer is not possible, Cfan is transmitter-only device");
}

CFan::~CFan()
{
}

void CFan::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Nothing to declare (transmitter-only device)
}

const shared::communication::CByteBuffer CFan::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF buffer;
   MEMCLEAR(buffer.FAN);

   buffer.FAN.packetlength = ENCODE_PACKET_LENGTH(FAN);
   buffer.FAN.packettype = pTypeFan;
   buffer.FAN.subtype = m_subType;
   buffer.FAN.seqnbr = seqNumberProvider->next();
   buffer.FAN.id1 = (unsigned char) (0xFF & (m_id >> 8));
   buffer.FAN.id2 = (unsigned char) (0xFF & m_id);
   buffer.FAN.cmnd = toProtocolState();
   buffer.FAN.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&buffer, GET_RBUF_STRUCT_SIZE(FAN));
}

void CFan::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   // Nothing to historize (transmitter-only device)
}

void CFan::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_id;
   m_deviceName = ssdeviceName.str();
}

void CFan::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeSiemensSF01: ssModel << "Siemens SF01"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }

   m_deviceModel = ssModel.str();
}

unsigned char CFan::toProtocolState() const
{
   if (m_lightCmd)
      return m_state.isOn() ? fan_sLight : 0x00;
   else
      return m_state.isOn() ? fan_sPlus : fan_sMin;
}

} // namespace rfxcomMessages