#include "stdafx.h"
#include "Lighting5.h"
#include "Lighting5LightwaveRf.h"
#include "Lighting5Livolo.h"
#include "Lighting5MdRemote.h"
#include "Lighting5OnOff.h"
#include <shared/Log.h>
#include "RareDeviceIdFilter.h"
#include "../MessageFilteredException.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType")));
      m_id = deviceDetails->get<unsigned int>("id");
      m_unitCode = static_cast<unsigned char>(deviceDetails->get<unsigned int>("unitCode"));
      m_subTypeManager->set(command);

      // Build device description
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_subType(static_cast<unsigned char>(subType)),
        m_deviceName(name),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
   {
      m_signalPower->set(0);

      createSubType(static_cast<unsigned char>(subType));

      m_id = manuallyDeviceCreationConfiguration->get<unsigned int>("id");

      switch (m_subType)
      {
      case sTypeMDREMOTE:
      case sTypeLivolo:
      case sTypeTRC02:
      case sTypeAoke:
      case sTypeTRC02_2:
      case sTypeRGB432W:
      case sTypeMDREMOTE107:
      case sTypeLegrandCAD:
      case sTypeMDREMOTE108:
         m_unitCode = 0;
         break;
      default:
         m_unitCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration->get<unsigned int>("unitCode"));
         break;
      }

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->reset();
   }

   CLighting5::CLighting5(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize,
                          boost::shared_ptr<IUnsecuredProtocolFilter> messageFilter)
      : m_messageFilter(messageFilter),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
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
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.LIGHTING5.rssi));

      // Build device description
      buildDeviceName();
      buildDeviceDetails();
   }

   CLighting5::~CLighting5()
   {
   }

   boost::shared_ptr<IUnsecuredProtocolFilter> CLighting5::createFilter()
   {
      return boost::make_shared<CRareDeviceIdFilter>(3,
                                                     boost::posix_time::minutes(1));
   }

   void CLighting5::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeLighting5);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("id", m_id);
         m_deviceDetails->set("unitCode", m_unitCode);
      }
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
      case sTypeLivolo1to10: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Livolo Appliance");
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
      case sTypeKangtai: m_subTypeManager = boost::make_shared<CLighting5OnOff>("Kangtai, Cotech");
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.push_back(m_subTypeManager->keyword());
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CLighting5::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
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
      rbuf.LIGHTING5.rssi = 0;
      rbuf.LIGHTING5.filler = 0;

      // Some sub-protocols need several messages
      auto buffers(boost::make_shared<std::queue<shared::communication::CByteBuffer>>());
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

   void CLighting5::filter() const
   {
      if (m_messageFilter && !m_messageFilter->isValid(m_deviceName))
         throw CMessageFilteredException((boost::format("Receive unknown device (id %1%) for unsecured protocol (LIGHTING5 / %2%), may be a transmission error : ignored")
            % m_id % m_subTypeManager->getModel()).str());
   }

   void CLighting5::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_subTypeManager->getModel(), m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_subTypeManager->getModel() << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CLighting5::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting5::keywords()
   {
      return m_keywords;
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
