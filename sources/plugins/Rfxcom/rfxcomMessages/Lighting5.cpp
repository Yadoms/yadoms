#include "stdafx.h"
#include "Lighting5.h"
#include "Lighting5LightwaveRf.h"
#include "Lighting5Livolo.h"
#include "Lighting5MdRemote.h"
#include "Lighting5OnOff.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_id = deviceDetails.get<unsigned int>("id");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      declare(api);
      m_subTypeManager->set(command);
   }

   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      declare(api);
      m_subTypeManager->reset();
   }

   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting5,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING5),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.LIGHTING5.subtype);
      m_id = rbuf.LIGHTING5.id1 << 16 | rbuf.LIGHTING5.id2 << 8 | rbuf.LIGHTING5.id3;
      m_unitCode = rbuf.LIGHTING5.unitcode;
      m_subTypeManager->setFromProtocolState(rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING5.signalPower));

      declare(api);
   }

   CLighting5::~CLighting5()
   {
   }

   void CLighting5::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case sTypeLightwaveRF: m_subTypeManager = boost::make_shared<CLighting5LightwaveRf>();
         break;
      case sTypeEMW100: m_subTypeManager = boost::make_shared<CLighting5OnOff>("EMW100 GAO/Everflourish");
         break;
      case sTypeBBSB: m_subTypeManager = boost::make_shared<CLighting5OnOff>("BBSB new types");
         break;
      case sTypeRSL: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Conrad RSL2");
         break;
      case sTypeMDREMOTE: m_subTypeManager = boost::make_shared<CLighting5MdRemote>("MDREMOTE 106 LED dimmer");
         break;
      case sTypeLivolo: m_subTypeManager = boost::make_shared<CLighting5Livolo>();
         break; // Limited support of Livolo (just ON/OFF), as we can't know exact type of module
      case sTypeTRC02: m_subTypeManager = boost::make_shared<CLighting5OnOff>("RGB TRC02 (2 batt)");
         break;
      case sTypeAoke: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Aoke Relay");
         break;
      case sTypeTRC02_2: m_subTypeManager = boost::make_shared<CLighting5OnOff>("RGB TRC02 (3 batt)");
         break;
      case sTypeEurodomest: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Eurodomest");
         break;
      case sTypeLivoloAppliance: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Livolo Appliance");
         break;
      case sTypeRGB432W: m_subTypeManager = boost::make_shared<CLighting5OnOff>("RGB432W");
         break;
      case sTypeMDREMOTE107: m_subTypeManager = boost::make_shared<CLighting5OnOff>("MD remote 107 LED");
         break;
      case sTypeLegrandCAD: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Legrand CAD");
         break;
      case sTypeAvantek: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Avantek");
         break;
      case sTypeIT: m_subTypeManager = boost::make_shared<CLighting5OnOff>("IT (Intertek,FA500,PROmax…)");
         break;
      case sTypeMDREMOTE108: m_subTypeManager = boost::make_shared<CLighting5MdRemote>("MDREMOTE 108 LED dimmer");
         break;
      case sTypeKangtai: m_subTypeManager = boost::make_shared<CLighting5MdRemote>("Kangtai, Cotech");
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.push_back(m_subTypeManager->keyword());
   }

   void CLighting5::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeLighting5);
         details.set("subType", m_subType);
         details.set("id", m_id);
         details.set("unitCode", m_unitCode);
         api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_keywords, details);
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
      rbuf.LIGHTING5.id1 = static_cast<unsigned char>(0xFF & (m_id >> 16));
      rbuf.LIGHTING5.id2 = static_cast<unsigned char>(0xFF & (m_id >> 8));
      rbuf.LIGHTING5.id3 = static_cast<unsigned char>(0xFF & m_id);
      rbuf.LIGHTING5.unitcode = m_unitCode;
      rbuf.LIGHTING5.signalPower = 0;
      rbuf.LIGHTING5.filler = 0;

      // Some sub-protocols need several messages
      auto buffers(boost::make_shared<std::queue<shared::communication::CByteBuffer> >());
      for (size_t idxMessage = 0; idxMessage < m_subTypeManager->getMessageNb(); ++idxMessage)
      {
         m_subTypeManager->toProtocolState(idxMessage, rbuf.LIGHTING5.cmnd, rbuf.LIGHTING5.level);
         buffers->push(toBuffer(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING5)));
      }
      return buffers;
   }

   void CLighting5::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CLighting5::getDeviceName() const
   {
      return m_deviceName;
   }

   void CLighting5::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages


