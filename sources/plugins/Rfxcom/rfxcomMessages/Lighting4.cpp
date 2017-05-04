#include "stdafx.h"
#include "Lighting4.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_keywords({ m_keyword , m_rssi })
   {
      m_rssi->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");

      Init(api);
   }

   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_keywords({ m_keyword , m_rssi })
   {
      m_rssi->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypePT2262)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

      Init(api);
   }

   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_keywords({ m_keyword , m_rssi })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting4,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING4),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.LIGHTING4.subtype;
      m_id = rbuf.LIGHTING4.cmd1 << 16 | rbuf.LIGHTING4.cmd2 << 8 | rbuf.LIGHTING4.cmd3;
      m_rssi->set(NormalizeRssiLevel(rbuf.LIGHTING4.rssi));

      Init(api);
   }

   CLighting4::~CLighting4()
   {
   }

   void CLighting4::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeLighting4);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CLighting4::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.LIGHTING4);

      rbuf.LIGHTING4.packetlength = ENCODE_PACKET_LENGTH(LIGHTING4);
      rbuf.LIGHTING4.packettype = pTypeLighting4;
      rbuf.LIGHTING4.subtype = m_subType;
      rbuf.LIGHTING4.seqnbr = seqNumberProvider->next();
      rbuf.LIGHTING4.cmd1 = static_cast<unsigned char>(0xFF & (m_id >> 16));
      rbuf.LIGHTING4.cmd2 = static_cast<unsigned char>(0xFF & (m_id >> 8));
      rbuf.LIGHTING4.cmd3 = static_cast<unsigned char>(0xFF & m_id);
      unsigned short pulse = 1400; // See RFXCom specification
      rbuf.LIGHTING4.pulseHigh = static_cast<unsigned char>(0xFF & (pulse >> 8));
      rbuf.LIGHTING4.pulseLow = static_cast<unsigned char>(0xFF & pulse);
      rbuf.LIGHTING4.rssi = 0;
      rbuf.LIGHTING4.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING4));
   }

   void CLighting4::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CLighting4::getDeviceName() const
   {
      return m_deviceName;
   }

   void CLighting4::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CLighting4::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypePT2262: ssModel << "PT2262";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


