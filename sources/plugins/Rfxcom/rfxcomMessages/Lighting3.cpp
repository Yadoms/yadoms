#include "stdafx.h"
#include "Lighting3.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting3::CLighting3(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CDimmable>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      m_state->set(command);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_system = static_cast<unsigned char>(deviceDetails->get<unsigned int>("system"));
      m_channel = deviceDetails->get<unsigned short>("channel");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CLighting3::CLighting3(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_state(boost::make_shared<yApi::historization::CDimmable>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      m_state->set(false);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypeKoppla)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_system = static_cast<unsigned char>(manuallyDeviceCreationConfiguration->get<unsigned int>("system"));
      m_channel = manuallyDeviceCreationConfiguration->get<unsigned short>("channel");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CLighting3::CLighting3(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CDimmable>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting3,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING3),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.LIGHTING3.subtype;
      m_system = rbuf.LIGHTING3.system;
      m_channel = rbuf.LIGHTING3.channel8_1 & (rbuf.LIGHTING3.channel10_9 << 8);
      m_state->set(fromProtocolState(rbuf.LIGHTING3.cmnd));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.LIGHTING2.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CLighting3::~CLighting3()
   {
   }

   void CLighting3::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeLighting3);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("system", m_system);
         m_deviceDetails->set("channel", m_channel);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CLighting3::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.LIGHTING3);

      rbuf.LIGHTING3.packetlength = ENCODE_PACKET_LENGTH(LIGHTING3);
      rbuf.LIGHTING3.packettype = pTypeLighting3;
      rbuf.LIGHTING3.subtype = m_subType;
      rbuf.LIGHTING3.seqnbr = seqNumberProvider->next();
      rbuf.LIGHTING3.system = m_system;
      rbuf.LIGHTING3.channel8_1 = static_cast<unsigned char>(m_channel & 0xFF);
      rbuf.LIGHTING3.channel10_9 = static_cast<unsigned char>((m_channel & 0xFF00) >> 8);
      rbuf.LIGHTING3.cmnd = toProtocolState(*m_state);
      rbuf.LIGHTING3.rssi = 0;
      rbuf.LIGHTING3.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING3));
   }

   void CLighting3::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CLighting3::filter() const
   {
   }

   void CLighting3::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CLighting3::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting3::keywords()
   {
      return m_keywords;
   }

   void CLighting3::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<unsigned int>(m_system) << "." << static_cast<unsigned int>(m_channel);
      m_deviceName = ssdeviceName.str();
   }

   void CLighting3::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeKoppla: ssModel << "Ikea Koppla";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CLighting3::toProtocolState(const yApi::historization::CDimmable& switchState)
   {
      switch (switchState.switchLevel() / 10) // switchLevel returns value from 0 to 100
      {
      case 0: return light3_sOff;
      case 1: return light3_sLevel1;
      case 2: return light3_sLevel2;
      case 3: return light3_sLevel3;
      case 4: return light3_sLevel4;
      case 5: return light3_sLevel5;
      case 6: return light3_sLevel6;
      case 7: return light3_sLevel7;
      case 8: return light3_sLevel8;
      case 9: return light3_sLevel9;
      default: return light3_sOn;
      }
   }

   int CLighting3::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case light3_sOff: return 0;
      case light3_sLevel1: return 10;
      case light3_sLevel2: return 20;
      case light3_sLevel3: return 30;
      case light3_sLevel4: return 40;
      case light3_sLevel5: return 50;
      case light3_sLevel6: return 60;
      case light3_sLevel7: return 70;
      case light3_sLevel8: return 80;
      case light3_sLevel9: return 90;
      case light3_sOn: return 100;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages
