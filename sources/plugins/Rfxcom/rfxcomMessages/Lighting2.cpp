#include "stdafx.h"
#include "Lighting2.h"
#include "Lighting2OnOffOrDimmable.h"
#include "Lighting2OnOff.h"
#include <shared/Log.h>
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          boost::shared_ptr<const yApi::IDeviceCommand> command,
                          const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_subType = deviceDetails.get<unsigned char>("subType");
      m_houseCode = (m_subType == sTypeKambrook) ? deviceDetails.get<unsigned char>("houseCode") : 0;
      m_id = deviceDetails.get<unsigned int>("id");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      m_subTypeManager = createSubType(m_subType);
      m_subTypeManager->set(command);
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());

      m_signalPower->set(0);

      api->declareKeywords(command->getDevice(), m_keywords);
   }

   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_subType = static_cast<unsigned char>(subType);
      m_houseCode = m_subType == sTypeKambrook ? (manuallyDeviceCreationConfiguration.get<char>("houseCode", 0) - 'A') : 0;
      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");
      auto deviceType = manuallyDeviceCreationConfiguration.get<std::string>("type") == "onOff" ? ILighting2Subtype::kOnOff : ILighting2Subtype::kDimmable;
      m_deviceDetails = buildDeviceDetails(m_subType,
                                           m_houseCode,
                                           m_id,
                                           m_unitCode);

      m_subTypeManager = createSubType(m_subType,
                                       deviceType);
      m_subTypeManager->reset();
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());

      m_signalPower->set(0);

      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CLighting2::CLighting2(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting2,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING2),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = static_cast<unsigned char>(rbuf.LIGHTING2.subtype);
      m_subTypeManager = createSubType(m_subType);
      m_subTypeManager->idFromProtocol(rbuf.LIGHTING2.id1, rbuf.LIGHTING2.id2, rbuf.LIGHTING2.id3, rbuf.LIGHTING2.id4, m_houseCode, m_id);
      m_unitCode = rbuf.LIGHTING2.unitcode;
      m_subTypeManager->setFromProtocolState(rbuf.LIGHTING2.cmnd, rbuf.LIGHTING2.level);

      m_deviceName = buildDeviceName(m_subType,
                                     m_houseCode,
                                     m_id,
                                     m_unitCode);
      m_deviceDetails = buildDeviceDetails(m_subType,
                                           m_houseCode,
                                           m_id,
                                           m_unitCode);

      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());

      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING2.rssi));

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         auto model = m_subTypeManager->getModel();
         api->declareDevice(m_deviceName, model, model);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
         m_deviceDetails.printToLog(YADOMS_LOG(information));
      }

      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CLighting2::~CLighting2()
   {
   }

   boost::shared_ptr<ILighting2Subtype> CLighting2::createSubType(unsigned char subType,
                                                                  ILighting2Subtype::EDeviceType deviceType) const
   {
      switch (subType)
      {
      case sTypeAC: return boost::make_shared<CLighting2OnOffOrDimmable>("AC", deviceType);
      case sTypeHEU: return boost::make_shared<CLighting2OnOffOrDimmable>("HomeEasy EU", deviceType);
      case sTypeANSLUT: return boost::make_shared<CLighting2OnOffOrDimmable>("ANSLUT", deviceType);
      case sTypeKambrook: return boost::make_shared<CLighting2OnOff>("Kambrook RF3672 (Australia)");
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CLighting2::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
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

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting2::keywords()
   {
      return m_keywords;
   }

   std::string CLighting2::buildDeviceName(unsigned char subType,
                                           unsigned char houseCode,
                                           unsigned int id,
                                           unsigned char unitCode)
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(subType);
      if (subType == sTypeKambrook)
         ssdeviceName << "." << static_cast<unsigned int>(houseCode);
      ssdeviceName <<
         "." << static_cast<unsigned int>(id) <<
         "." << static_cast<unsigned int>(unitCode);
      return ssdeviceName.str();
   }

   shared::CDataContainer CLighting2::buildDeviceDetails(unsigned char subType,
                                                         unsigned char houseCode,
                                                         unsigned int id,
                                                         unsigned char unitCode)
   {
      shared::CDataContainer deviceDetails;
      if (deviceDetails.empty())
      {
         deviceDetails.set("type", pTypeLighting2);
         deviceDetails.set("subType", subType);
         if (subType == sTypeKambrook)
            deviceDetails.set("houseCode", houseCode);
         deviceDetails.set("id", id);
         deviceDetails.set("unitCode", unitCode);
      }
      return deviceDetails;
   }
} // namespace rfxcomMessages


