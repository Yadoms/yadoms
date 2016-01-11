#include "stdafx.h"
#include "Radiator1.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& keyword, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_day("day"), m_setPoint("setPoint"), m_rssi("rssi")
{
   if (boost::iequals(keyword, m_day.getKeyword()))
   {
      m_day.setCommand(command);
      m_dayNightCmd = true;
   }
   else if (boost::iequals(keyword, m_setPoint.getKeyword()))
   {
      m_setPoint.setCommand(command);
      m_dayNightCmd = false;
   }
   else
      throw shared::exception::CInvalidParameter(keyword);

   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");

   Init(context);
}

CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_day("day"), m_setPoint("setPoint"), m_rssi("rssi")
{
   m_day.set(false);
   m_setPoint.set(0.0);
   m_rssi.set(0);

   m_subType = subType;
   switch(m_subType)
   {
   case sTypeSmartwares:
      break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
}

CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_subType(0), m_unitCode(0), m_id(0), m_day("day"), m_setPoint("setPoint"), m_rssi("rssi")
{
   // Should not be called (transmitter-only device)
   BOOST_ASSERT_MSG(false, "Constructing CRadiator1 object from received buffer is not possible, CRadiator1 is transmitter-only device");
}

CRadiator1::~CRadiator1()
{
}

void CRadiator1::Init(boost::shared_ptr<yApi::IYPluginApi> context)
{
   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeRadiator1);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      context->declareKeyword(m_deviceName, m_day);
      context->declareKeyword(m_deviceName, m_setPoint);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CRadiator1::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.RADIATOR1);

   rbuf.RADIATOR1.packetlength = ENCODE_PACKET_LENGTH(RADIATOR1);
   rbuf.RADIATOR1.packettype = pTypeRadiator1;
   rbuf.RADIATOR1.subtype = m_subType;
   rbuf.RADIATOR1.seqnbr = seqNumberProvider->next();
   rbuf.RADIATOR1.id1 = static_cast<unsigned char>((m_id & 0xFF000000) >> 24);
   rbuf.RADIATOR1.id2 = static_cast<unsigned char>((m_id & 0xFF0000) >> 16);
   rbuf.RADIATOR1.id3 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
   rbuf.RADIATOR1.id4 = static_cast<unsigned char>(m_id & 0xFF);
   rbuf.RADIATOR1.unitcode = m_unitCode;
   toProtocolState(rbuf);
   rbuf.RADIATOR1.rssi = 0;
   rbuf.RADIATOR1.filler = 0;

   return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(RADIATOR1));
}

void CRadiator1::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   // Nothing to historize (transmitter-only device)
}

const std::string& CRadiator1::getDeviceName() const
{
   return m_deviceName;
}                     

void CRadiator1::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << static_cast<unsigned int>(m_id) << "." << static_cast<unsigned int>(m_unitCode);
   m_deviceName = ssdeviceName.str();
}

void CRadiator1::buildDeviceModel()
{
   std::ostringstream ssModel;

   switch(m_subType)
   {
   case sTypeSmartwares: ssModel << "Smartwares radiator valve"; break;
   default: ssModel << boost::lexical_cast<std::string>(m_subType); break;
   }
   
   m_deviceModel = ssModel.str();
}

void CRadiator1::toProtocolState(RBUF& radiator1) const
{
   if (m_dayNightCmd)
   {
      radiator1.RADIATOR1.cmnd = m_day.get() ? Radiator1_sDay : Radiator1_sNight;
      radiator1.RADIATOR1.temperature = 0;
      radiator1.RADIATOR1.tempPoint5 = 0;
   }
   else
   {
      radiator1.RADIATOR1.cmnd = Radiator1_sSetTemp;
      // Round set point to nearest 0.5
      double roundedSetPoint = floor(m_setPoint.get() * 2) / 2;
      double setPointIntegerPartAsFloat;
      int setPointDecimalPart = static_cast<int>(modf(roundedSetPoint, &setPointIntegerPartAsFloat));
      int setPointIntegerPart = static_cast<int>(setPointIntegerPartAsFloat);

      radiator1.RADIATOR1.temperature = static_cast<BYTE>(setPointIntegerPart);
      radiator1.RADIATOR1.tempPoint5 = setPointDecimalPart == 0 ? 0 : 5;
   }
}

} // namespace rfxcomMessages