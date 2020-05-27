#include "stdafx.h"
#include "Blinds1.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;


namespace rfxcomMessages
{
   CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& command,
                      const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_batteryLevel, m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
   {
      m_state->setCommand(command);
      m_batteryLevel->set(100);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_id = deviceDetails->get<unsigned int>("id");
      m_unitCode = static_cast<unsigned char>(deviceDetails->get<unsigned int>("unitCode"));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> api,
                      unsigned int subType,
                      const std::string& name,
                      const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_batteryLevel, m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
   {
      m_state->set(yApi::historization::ECurtainCommand::kStop);
      m_batteryLevel->set(100);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeBlindsT0:
      case sTypeBlindsT1:
      case sTypeBlindsT2:
      case sTypeBlindsT3:
      case sTypeBlindsT4:
      case sTypeBlindsT5:
      case sTypeBlindsT6:
      case sTypeBlindsT7:
      case sTypeBlindsT8:
      case sTypeBlindsT9:
      case sTypeBlindsT10:
      case sTypeBlindsT11:
      case sTypeBlindsT12:
      case sTypeBlindsT13:
      case sTypeBlindsT14:
      case sTypeBlindsT16:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_id = manuallyDeviceCreationConfiguration->get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration->getWithDefault<unsigned char>("unitCode", 0);

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CBlinds1::CBlinds1(boost::shared_ptr<yApi::IYPluginApi> api,
                      const RBUF& rbuf,
                      size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_batteryLevel, m_signalPower}),
        m_deviceDetails(shared::CDataContainer::make())
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeBlinds,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(BLINDS1),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.BLINDS1.subtype;

      switch (m_subType)
      {
      case sTypeBlindsT0:
      case sTypeBlindsT1:
      case sTypeBlindsT12:
      case sTypeBlindsT13:
      case sTypeBlindsT16:
         m_id = rbuf.BLINDS1.id2 << 8 | rbuf.BLINDS1.id3;
         break;

      case sTypeBlindsT2:
      case sTypeBlindsT3:
      case sTypeBlindsT4:
      case sTypeBlindsT5:
      case sTypeBlindsT8:
      case sTypeBlindsT10:
      case sTypeBlindsT11:
      case sTypeBlindsT14:
         m_id = rbuf.BLINDS1.id1 << 16 | rbuf.BLINDS1.id2 << 8 | rbuf.BLINDS1.id3;
         break;

      case sTypeBlindsT6:
      case sTypeBlindsT7:
         m_id = rbuf.BLINDS1.id1 << 24 | rbuf.BLINDS1.id2 << 16 | rbuf.BLINDS1.id3 << 8 | rbuf.BLINDS1.id4;
         break;

      case sTypeBlindsT9:
         m_id = rbuf.BLINDS1.id2 << 16 | rbuf.BLINDS1.id3 << 8 | rbuf.BLINDS1.id4;
         break;

      default:
         throw shared::exception::COutOfRange("Encode : subType is not supported");
      }

      switch (m_subType)
      {
      case sTypeBlindsT3:
      case sTypeBlindsT8:
      case sTypeBlindsT12:
      case sTypeBlindsT14:
         m_unitCode = rbuf.BLINDS1.unitcode + 1;
         break;
      case sTypeBlindsT0:
      case sTypeBlindsT1:
      case sTypeBlindsT2:
      case sTypeBlindsT4:
      case sTypeBlindsT5:
      case sTypeBlindsT6:
      case sTypeBlindsT7:
      case sTypeBlindsT9:
      case sTypeBlindsT10:
      case sTypeBlindsT11:
      case sTypeBlindsT13:
      case sTypeBlindsT16:
         m_unitCode = rbuf.BLINDS1.unitcode;
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_state->set(fromProtocolState(rbuf.BLINDS1.cmnd));
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.BLINDS1.filler)); // filler is specified as battery level in RFXtrx SDF.pdf
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.BLINDS1.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CBlinds1::~CBlinds1()
   {
   }

   void CBlinds1::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeBlinds);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("id", m_id);
         m_deviceDetails->set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CBlinds1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF buffer;
      MEMCLEAR(buffer.BLINDS1);

      buffer.BLINDS1.packetlength = ENCODE_PACKET_LENGTH(BLINDS1);
      buffer.BLINDS1.packettype = pTypeBlinds;
      buffer.BLINDS1.subtype = m_subType;
      buffer.BLINDS1.seqnbr = seqNumberProvider->next();

      switch (m_subType)
      {
      case sTypeBlindsT0:
      case sTypeBlindsT1:
      case sTypeBlindsT12:
      case sTypeBlindsT13:
      case sTypeBlindsT16:
         buffer.BLINDS1.id1 = 0;
         buffer.BLINDS1.id2 = static_cast<unsigned char>(0xFF & (m_id >> 8));
         buffer.BLINDS1.id3 = static_cast<unsigned char>(0xFF & m_id);
         buffer.BLINDS1.id4 = 0;
         break;

      case sTypeBlindsT2:
      case sTypeBlindsT3:
      case sTypeBlindsT4:
      case sTypeBlindsT5:
      case sTypeBlindsT8:
      case sTypeBlindsT10:
      case sTypeBlindsT11:
      case sTypeBlindsT14:
         buffer.BLINDS1.id1 = static_cast<unsigned char>(0xFF & (m_id >> 16));
         buffer.BLINDS1.id2 = static_cast<unsigned char>(0xFF & (m_id >> 8));
         buffer.BLINDS1.id3 = static_cast<unsigned char>(0xFF & m_id);
         buffer.BLINDS1.id4 = 0;
         break;

      case sTypeBlindsT6:
      case sTypeBlindsT7:
         buffer.BLINDS1.id1 = static_cast<unsigned char>(0xFF & (m_id >> 24));
         buffer.BLINDS1.id2 = static_cast<unsigned char>(0xFF & (m_id >> 16));
         buffer.BLINDS1.id3 = static_cast<unsigned char>(0xFF & (m_id >> 8));
         buffer.BLINDS1.id4 = static_cast<unsigned char>(0xFF & m_id);
         break;

      case sTypeBlindsT9:
         buffer.BLINDS1.id1 = 0;
         buffer.BLINDS1.id2 = static_cast<unsigned char>(0xFF & (m_id >> 16));
         buffer.BLINDS1.id3 = static_cast<unsigned char>(0xFF & (m_id >> 8));
         buffer.BLINDS1.id4 = static_cast<unsigned char>(0xFF & m_id);
         break;

      default:
         throw shared::exception::COutOfRange("Encode : subType is not supported");
      }

      switch (m_subType)
      {
      case sTypeBlindsT3:
      case sTypeBlindsT8:
      case sTypeBlindsT12:
      case sTypeBlindsT14:
         buffer.BLINDS1.unitcode = m_unitCode - 1;
         break;
      case sTypeBlindsT0:
      case sTypeBlindsT1:
      case sTypeBlindsT2:
      case sTypeBlindsT4:
      case sTypeBlindsT5:
      case sTypeBlindsT6:
      case sTypeBlindsT7:
      case sTypeBlindsT9:
      case sTypeBlindsT10:
      case sTypeBlindsT11:
      case sTypeBlindsT13:
      case sTypeBlindsT16:
         buffer.BLINDS1.unitcode = m_unitCode;
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      buffer.BLINDS1.cmnd = toProtocolState(*m_state);
      buffer.BLINDS1.rssi = 0;
      buffer.BLINDS1.filler = 0;

      return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(BLINDS1));
   }

   void CBlinds1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CBlinds1::filter() const
   {
   }

   void CBlinds1::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName,
                         m_deviceModel,
                         m_deviceModel,
                         m_keywords,
                         m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CBlinds1::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CBlinds1::keywords()
   {
      return m_keywords;
   }

   void CBlinds1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<unsigned int>(m_id) << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CBlinds1::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeBlindsT0: ssModel << "RollerTrol, Hasta new";
         break;
      case sTypeBlindsT1: ssModel << "Hasta old";
         break;
      case sTypeBlindsT2: ssModel << "A-OK RF01";
         break;
      case sTypeBlindsT3: ssModel << "A-OK AC114/AC123";
         break;
      case sTypeBlindsT4: ssModel << "RAEX YR1326";
         break;
      case sTypeBlindsT5: ssModel << "Media Mount";
         break;
      case sTypeBlindsT6: ssModel << "DC106, YOOHA, Rohrmotor24 RMF";
         break;
      case sTypeBlindsT7: ssModel << "Forest";
         break;
      case sTypeBlindsT8: ssModel << "Chamberlain CS4330CN";
         break;
      case sTypeBlindsT9: ssModel << "Sunpery/BTX";
         break;
      case sTypeBlindsT10: ssModel << "Dolat DLM-1, Topstar";
         break;
      case sTypeBlindsT11: ssModel << "ASP";
         break;
      case sTypeBlindsT12: ssModel << "Confexx CNF24-2435";
         break;
      case sTypeBlindsT13: ssModel << "Screenline";
         break;
      case sTypeBlindsT14: ssModel << "Hualite";
         break;
      case sTypeBlindsT16: ssModel << "Zemismart";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   yApi::historization::ECurtainCommand CBlinds1::fromProtocolState(unsigned char protocolCmnd)
   {
      switch (protocolCmnd)
      {
      case blinds_sOpen: return yApi::historization::ECurtainCommand::kOpen;
      case blinds_sClose: return yApi::historization::ECurtainCommand::kClose;
      case blinds_sStop: return yApi::historization::ECurtainCommand::kStop;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolCmnd));
      }
   }

   unsigned char CBlinds1::toProtocolState(const yApi::historization::CCurtain& curtainState)
   {
      switch (curtainState.get()())
      {
      case yApi::historization::ECurtainCommand::kOpenValue: return blinds_sOpen;
      case yApi::historization::ECurtainCommand::kCloseValue: return blinds_sClose;
      case yApi::historization::ECurtainCommand::kStopValue: return blinds_sStop;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(curtainState.get()()));
      }
   }
} // namespace rfxcomMessages
