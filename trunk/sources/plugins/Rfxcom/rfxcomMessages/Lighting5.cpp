#include "stdafx.h"
#include "Lighting5.h"
#include <shared/plugin/yadomsApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include "Lighting5LightwaveRfKeyword.h"
#include "Lighting5MdRemoteKeyword.h"
#include "Lighting5OnOffKeyword.h"
//TODO support � minima : il faudrait g�rer l'�clairage led color� mieux que �a (cr�er historizer et tout)
// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace rfxcomMessages
{

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, const shared::CDataContainer& command, const shared::CDataContainer& deviceDetails)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = deviceDetails.get<unsigned char>("subType");
   m_id = deviceDetails.get<unsigned int>("id");
   m_unitCode = deviceDetails.get<unsigned char>("unitCode");
 
   Init(context);
   m_keyword->set(command);
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, unsigned char subType, const shared::CDataContainer& manuallyDeviceCreationConfiguration)
   :m_rssi("rssi")
{
   m_rssi.set(0);

   m_subType = subType;

   m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
   m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

   Init(context);
   m_keyword->default();
}

CLighting5::CLighting5(boost::shared_ptr<yApi::IYadomsApi> context, const RBUF& rbuf, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
   :m_rssi("rssi")
{
   CheckReceivedMessage(rbuf, pTypeLighting5, GET_RBUF_STRUCT_SIZE(LIGHTING5), DONT_CHECK_SEQUENCE_NUMBER);

   m_subType = rbuf.LIGHTING5.subtype;
   m_id = rbuf.LIGHTING5.id1 << 16 | rbuf.LIGHTING5.id2 << 8 | rbuf.LIGHTING5.id3;
   m_unitCode = rbuf.LIGHTING5.unitcode;
   m_keyword->setFromProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
   m_rssi.set(NormalizeRssiLevel(rbuf.LIGHTING5.rssi));

   Init(context);
}

CLighting5::~CLighting5()
{
}

void CLighting5::Init(boost::shared_ptr<yApi::IYadomsApi> context)
{
   switch(m_subType)
   {
   case sTypeLightwaveRF : m_keyword.reset(new CLighting5LightwaveRfKeyword()); break;
   case sTypeEMW100      : m_keyword.reset(new CLighting5OnOffKeyword("EMW100 GAO/Everflourish")); break;
   case sTypeBBSB        : m_keyword.reset(new CLighting5OnOffKeyword("BBSB new types")); break;
   case sTypeRSL         : m_keyword.reset(new CLighting5OnOffKeyword("Conrad RSL2")); break;
   case sTypeMDREMOTE    : m_keyword.reset(new CLighting5MdRemoteKeyword()); break;
   case sTypeLivolo      : m_keyword.reset(new CLighting5OnOffKeyword("Livolo")); break; // Limited support of Livolo (just ON/OFF), as we can't now exact type of module
   case sTypeTRC02       : m_keyword.reset(new CLighting5OnOffKeyword("RGB TRC02")); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }

   // Build device description
   buildDeviceModel();
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeLighting5);
      details.set("subType", m_subType);
      details.set("id", m_id);
      details.set("unitCode", m_unitCode);
      context->declareDevice(m_deviceName, m_deviceModel, details.serialize());

      m_keyword->declare(context, m_deviceName);
      context->declareKeyword(m_deviceName, m_rssi);
   }
}

const shared::communication::CByteBuffer CLighting5::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
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
   m_keyword->toProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
   rbuf.LIGHTING5.rssi = 0;
   rbuf.LIGHTING5.filler = 0;

   return shared::communication::CByteBuffer((BYTE*)&rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING5));
}

void CLighting5::historizeData(boost::shared_ptr<yApi::IYadomsApi> context) const
{
   m_keyword->historize(context, m_deviceName);
   context->historizeData(m_deviceName, m_rssi);
}

void CLighting5::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   ssdeviceName << m_deviceModel << "." << (unsigned int)m_id << "." << (unsigned int)m_unitCode;
   m_deviceName = ssdeviceName.str();
}

void CLighting5::buildDeviceModel()
{
   m_deviceModel = m_keyword->getModel();
}

} // namespace rfxcomMessages