#include "stdafx.h"
#include "Lighting5.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Lighting5LightwaveRf.h"
#include "Lighting5Livolo.h"
#include "Lighting5MdRemote.h"
#include "Lighting5OnOff.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(deviceDetails.get<unsigned char>("subType"));
   m_id = deviceDetails.get<unsigned int>("id");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");
 
   declare(context);
   m_subTypeManager->set(command);
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   createSubType(subType);

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   declare(context);
   m_subTypeManager->reset();
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeLighting5, GET_RBUF_STRUCT_SIZE(LIGHTING5), DONT_CHECK_SEQUENCE_NUMBER);

   createSubType(rbuf.LIGHTING5.subtype);
   m_id = rbuf.LIGHTING5.id1 << 16 | rbuf.LIGHTING5.id2 << 8 | rbuf.LIGHTING5.id3;
   m_unitCode = rbuf.LIGHTING5.unitcode;
   m_subTypeManager->setFromProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING5.rssi));

   declare(context);
}

CLighting5::~CLighting5()
{
}

void CLighting5::createSubType(unsigned char subType)
{
   m_subType = subType;
   switch(m_subType)
   {
   case sTypeLightwaveRF      : m_subTypeManager.reset(new CLighting5LightwaveRfKeyword()); break;
   case sTypeEMW100           : m_subTypeManager.reset(new CLighting5OnOff("EMW100 GAO/Everflourish")); break;
   case sTypeBBSB             : m_subTypeManager.reset(new CLighting5OnOff("BBSB new types")); break;
   case sTypeRSL              : m_subTypeManager.reset(new CLighting5OnOff("Conrad RSL2")); break;
   case sTypeMDREMOTE         : m_subTypeManager.reset(new CLighting5MdRemoteKeyword()); break;
   case sTypeLivolo           : m_subTypeManager.reset(new CLighting5Livolo()); break; // Limited support of Livolo (just ON/OFF), as we can't now exact type of module
   case sTypeTRC02            : m_subTypeManager.reset(new CLighting5OnOff("RGB TRC02 (2 batt)")); break;
   case sTypeAoke             : m_subTypeManager.reset(new CLighting5OnOff("Aoke Relay")); break;
   case sTypeTRC02_2          : m_subTypeManager.reset(new CLighting5OnOff("RGB TRC02 (3 batt)")); break;
   case sTypeEurodomest       : m_subTypeManager.reset(new CLighting5OnOff("Eurodomest")); break;
   case sTypeLivoloAppliance  : m_subTypeManager.reset(new CLighting5OnOff("Livolo Appliance")); break;
   case sTypeRGB432W          : m_subTypeManager.reset(new CLighting5OnOff("RGB432W")); break;
   case sTypeMDREMOTE107      : m_subTypeManager.reset(new CLighting5OnOff("MD remote 107 LED")); break;
   case sTypeLegrandCAD       : m_subTypeManager.reset(new CLighting5OnOff("Legrand CAD")); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CLighting5::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!m_subTypeManager)
      throw shared::exception::CException("m_subTypeManager must be initialized");

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting5);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_subTypeManager->getModel(), details);

      m_subTypeManager->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CLighting5::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   RBUF rbuf;
   MEMCLEAR(rbuf.LIGHTING5);

   rbuf.LIGHTING5.packetlength = ENCODE_PACKET_LENGTH(LIGHTING5);
   rbuf.LIGHTING5.packettype = pTypeLighting5;
   rbuf.LIGHTING5.subtype = m_subType;
   rbuf.LIGHTING5.seqnbr = seqNumberProvider->next();
   rbuf.LIGHTING5.id1 = (unsigned char) (0xFF & (m_id >> 16));
   rbuf.LIGHTING5.id2 = (unsigned char) (0xFF & (m_id >> 8));
   rbuf.LIGHTING5.id3 = (unsigned char) (0xFF & m_id);
   rbuf.LIGHTING5.unitcode = m_unitCode;
   rbuf.LIGHTING5.rssi = 0;
   rbuf.LIGHTING5.filler = 0;

   // Some sub-protocols need several messages
   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > buffers(new std::queue<shared::communication::CByteBuffer>);
   for (size_t idxMessage = 0 ; idxMessage < m_subTypeManager->getMessageNb() ; ++ idxMessage)
   {
      m_subTypeManager->toProtocolState(idxMessage, rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
      buffers->push(toBuffer(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING5)));
   }
   return buffers;
}

void CLighting5::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   m_subTypeManager->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

const std::string& CLighting5::getDeviceName() const
{
   return m_deviceName;
}                     

void CLighting5::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << (unsigned int)m_subType << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages