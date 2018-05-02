#include "stdafx.h"
#include "Lighting4.h"
#include "../MessageFilteredException.hpp"
#include "RareDeviceIdFilter.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_keyword , m_signalPower})
   {
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_keyword , m_signalPower})
   {
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypePT2262)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CLighting4::CLighting4(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize,
                          boost::shared_ptr<IUnsecuredProtocolFilter> messageFilter)
      : m_messageFilter(messageFilter),
        m_keyword(boost::make_shared<yApi::historization::CEvent>("event")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_keyword , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting4,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING4),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.LIGHTING4.subtype;
      m_id = rbuf.LIGHTING4.cmd1 << 16 | rbuf.LIGHTING4.cmd2 << 8 | rbuf.LIGHTING4.cmd3;
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING4.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CLighting4::~CLighting4()
   {
   }

   boost::shared_ptr<IUnsecuredProtocolFilter> CLighting4::createFilter()
   {
      return boost::make_shared<CRareDeviceIdFilter>(3,
                                                     boost::posix_time::minutes(1));
   }

   void CLighting4::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeLighting4);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CLighting4::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
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

   void CLighting4::filter() const
   {
      if (m_messageFilter && !m_messageFilter->isValid(m_deviceName))
         throw CMessageFilteredException((boost::format("Receive unknown device (id %1%) for unsecured protocol (LIGHTING4 / %2%), may be a transmission error : ignored")
            % m_id % m_deviceModel).str());
   }

   void CLighting4::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails.printToLog(YADOMS_LOG(information));
   }

   const std::string& CLighting4::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting4::keywords()
   {
      return m_keywords;
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
