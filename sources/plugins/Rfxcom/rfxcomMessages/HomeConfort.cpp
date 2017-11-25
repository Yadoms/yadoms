#include "stdafx.h"
#include "HomeConfort.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CHomeConfort::CHomeConfort(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& command,
                              const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      m_state->setCommand(command);
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");
      m_houseCode = deviceDetails.get<char>("houseCode");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CHomeConfort::CHomeConfort(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
      const std::string& name,
                              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
      m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      m_state->set(false);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypeHomeConfortTEL010)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_houseCode = manuallyDeviceCreationConfiguration.get<char>("houseCode");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CHomeConfort::CHomeConfort(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeHomeConfort,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(HOMECONFORT),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.HOMECONFORT.subtype;
      m_id = (rbuf.HOMECONFORT.id1 << 16) | (rbuf.HOMECONFORT.id2 << 8) | rbuf.HOMECONFORT.id3;
      m_houseCode = rbuf.HOMECONFORT.housecode;
      m_unitCode = rbuf.HOMECONFORT.unitcode;
      m_state->set(fromProtocolState(rbuf.HOMECONFORT.cmnd));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.HOMECONFORT.rssi));
      
      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         m_deviceDetails.printToLog(YADOMS_LOG(information));         
      }
   }

   CHomeConfort::~CHomeConfort()
   {
   }

   void CHomeConfort::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeHomeConfort);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
         m_deviceDetails.set("houseCode", m_houseCode);
         m_deviceDetails.set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CHomeConfort::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.HOMECONFORT);

      rbuf.HOMECONFORT.packetlength = ENCODE_PACKET_LENGTH(HOMECONFORT);
      rbuf.HOMECONFORT.packettype = pTypeHomeConfort;
      rbuf.HOMECONFORT.subtype = m_subType;
      rbuf.HOMECONFORT.seqnbr = seqNumberProvider->next();
      rbuf.HOMECONFORT.id1 = static_cast<unsigned char>((m_id & 0x070000) >> 16);
      rbuf.HOMECONFORT.id2 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.HOMECONFORT.id3 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.HOMECONFORT.housecode = m_houseCode;
      rbuf.HOMECONFORT.unitcode = m_unitCode;
      rbuf.HOMECONFORT.cmnd = toProtocolState(*m_state);
      rbuf.HOMECONFORT.rssi = 0;
      rbuf.HOMECONFORT.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(HOMECONFORT));
   }

   void CHomeConfort::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CHomeConfort::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CHomeConfort::keywords()
   {
      return m_keywords;
   }

   void CHomeConfort::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << m_houseCode << "." << m_id << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CHomeConfort::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeHomeConfortTEL010: ssModel << "Blyss";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CHomeConfort::toProtocolState(const yApi::historization::CSwitch& switchState)
   {
      return switchState.get() ? HomeConfort_sOn : HomeConfort_sOff;
   }

   bool CHomeConfort::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case HomeConfort_sOn:
      case HomeConfort_sGroupOn:
         return true;

      case HomeConfort_sOff:
      case HomeConfort_sGroupOff:
         return false;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages


