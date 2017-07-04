#include "stdafx.h"
#include "Lighting2.h"
#include "Lighting2Dimmable.h"
#include "Lighting2OnOff.h"
#include <shared/Log.h>
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_houseCode = (m_subType == sTypeKambrook) ? deviceDetails.get<unsigned char>("houseCode") : 0;
      m_id = deviceDetails.get<unsigned int>("id");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      declare(api);
      m_subTypeManager->set(command);
   }

   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
      const std::string& name,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));

      m_houseCode = (m_subType == sTypeKambrook) ? (manuallyDeviceCreationConfiguration.get<char>("houseCode", 0) - 'A') : 0;
      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->reset();
   }

   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting2,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING2),
                           DONT_CHECK_SEQUENCE_NUMBER);

      createSubType(rbuf.LIGHTING2.subtype);
      m_subTypeManager->idFromProtocol(rbuf.LIGHTING2.id1, rbuf.LIGHTING2.id2, rbuf.LIGHTING2.id3, rbuf.LIGHTING2.id4, m_houseCode, m_id);
      m_subTypeManager->setFromProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);
      m_unitCode = rbuf.LIGHTING2.unitcode;
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING2.rssi));

      declare(api);
   }

   CLighting2::~CLighting2()
   {
   }

   void CLighting2::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeLighting2);
         m_deviceDetails.set("subType", m_subType);
         if (m_subType == sTypeKambrook)
            m_deviceDetails.set("houseCode", m_houseCode);
         m_deviceDetails.set("id", m_id);
         m_deviceDetails.set("unitCode", m_unitCode);
      }
   }

   void CLighting2::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case sTypeAC: m_subTypeManager = boost::make_shared<CLighting2Dimmable>("AC");
         break;
      case sTypeHEU: m_subTypeManager = boost::make_shared<CLighting2Dimmable>("HomeEasy EU");
         break;
      case sTypeANSLUT: m_subTypeManager = boost::make_shared<CLighting2Dimmable>("ANSLUT");
         break;
      case sTypeKambrook: m_subTypeManager = boost::make_shared<CLighting2OnOff>("Kambrook RF3672 (Australia)");
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.push_back(m_subTypeManager->keyword());
   }

   void CLighting2::declare(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Build device description
      buildDeviceName();
      buildDeviceDetails();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         auto model = m_subTypeManager->getModel();
         api->declareDevice(m_deviceName, model, model, m_keywords, m_deviceDetails);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
         m_deviceDetails.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CLighting2::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.LIGHTING2);

      rbuf.LIGHTING2.packetlength = ENCODE_PACKET_LENGTH(LIGHTING2);
      rbuf.LIGHTING2.packettype = pTypeLighting2;
      rbuf.LIGHTING2.subtype = m_subType;
      rbuf.LIGHTING2.seqnbr = seqNumberProvider->next();
      m_subTypeManager->idToProtocol(m_houseCode, m_id, rbuf.LIGHTING2.id1, rbuf.LIGHTING2.id2, rbuf.LIGHTING2.id3, rbuf.LIGHTING2.id4);
      m_subTypeManager->toProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);
      rbuf.LIGHTING2.unitcode = m_unitCode;
      rbuf.LIGHTING2.rssi = 0;
      rbuf.LIGHTING2.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING2));
   }

   void CLighting2::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CLighting2::getDeviceName() const
   {
      return m_deviceName;
   }

   void CLighting2::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType);
      if (m_subType == sTypeKambrook)
         ssdeviceName << "." << static_cast<unsigned int>(m_houseCode);
      ssdeviceName <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages


