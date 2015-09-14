#include "stdafx.h"
#include "Fan.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_lightCmd(false), m_light("light"), m_fan("fan")
{
   if (boost::iequals(keyword, m_light.getKeyword()))
   {
      m_light.set(command);
      m_lightCmd = true;
   }
   else if (boost::iequals(keyword, m_fan.getKeyword()))
   {
      m_fan.set(command);
      m_lightCmd = false;
   }
   else
      throw shared::exception::CInvalidParameter(keyword);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");

   Init(context);
}

CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_lightCmd(false), m_light("light"), m_fan("fan")
{
   m_light.set(false);
   m_fan.set(false);

   m_subType = subType;
   if (m_subType != sTypeSiemensSF01)
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

   Init(context);
}

CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_subType(0), m_id(0), m_lightCmd(false), m_light("light"), m_fan("fan")
{
   // Should not be called (transmitter-only device)
   BOOST_ASSERT_MSG(false, "Constructing CFan object from received buffer is not possible, Cfan is transmitter-only device");
}

CFan::~CFan()
{
}

void CFan::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeFan);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_light);
      context->declareKeyword(m_deviceName, m_fan);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CFan::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
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

   return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(FAN));
}

void CFan::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   // Nothing to historize (transmitter-only device)
}

const std::string& CFan::getDeviceName() const
{
   return m_deviceName;
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
      return m_light.get() ? fan_sLight : 0x00;
   else
      return m_fan.get() ? fan_sPlus : fan_sMin;
}

} // namespace rfxcomMessages