#include "stdafx.h"
#include "Thermostat3.h"
#include "Thermostat3MertikG6RH4T1.h"
#include "Thermostat3MertikG6RH4TB.h"
#include "Thermostat3MertikG6RH4TD.h"
#include "Thermostat3MertikG6RH4S.h"
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& keyword,
                              const std::string& command,
                              const shared::CDataContainer& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_unitCode = deviceDetails.get<unsigned int>("unitCode");

      Init(api);
      m_subTypeManager->set(keyword, command);
   }

   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);

      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned int>("unitCode");

      Init(api);
      m_subTypeManager->reset();
   }

   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeThermostat3,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(THERMOSTAT3),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.THERMOSTAT3.subtype;
      m_unitCode = rbuf.THERMOSTAT3.unitcode1 << 16 | rbuf.THERMOSTAT3.unitcode2 << 8 | rbuf.THERMOSTAT3.unitcode3;
      m_subTypeManager->setFromProtocolState(rbuf.THERMOSTAT3.cmnd);
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.THERMOSTAT3.signalPower));

      Init(api);
   }

   CThermostat3::~CThermostat3()
   {
   }

   void CThermostat3::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      switch (m_subType)
      {
      case sTypeMertikG6RH4T1: m_subTypeManager = boost::make_shared<CThermostat3MertikG6RH4T1>();
         break;
      case sTypeMertikG6RH4TB: m_subTypeManager = boost::make_shared<CThermostat3MertikG6RH4TB>();
         break;
      case sTypeMertikG6RH4TD: m_subTypeManager = boost::make_shared<CThermostat3MertikG6RH4TD>();
         break;
      case sTypeMertikG6RH4S: m_subTypeManager = boost::make_shared<CThermostat3MertikG6RH4S>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());

      // Build device description
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeThermostat3);
         details.set("subType", m_subType);
         details.set("unitCode", m_unitCode);
         api->declareDevice(m_deviceName, m_subTypeManager->getModel(), m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CThermostat3::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.THERMOSTAT3);

      rbuf.THERMOSTAT3.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT3);
      rbuf.THERMOSTAT3.packettype = pTypeThermostat3;
      rbuf.THERMOSTAT3.subtype = m_subType;
      rbuf.THERMOSTAT3.seqnbr = seqNumberProvider->next();
      rbuf.THERMOSTAT3.unitcode1 = static_cast<unsigned char>(0xFF & (m_unitCode >> 16));
      rbuf.THERMOSTAT3.unitcode2 = static_cast<unsigned char>(0xFF & (m_unitCode >> 8));
      rbuf.THERMOSTAT3.unitcode3 = static_cast<unsigned char>(0xFF & m_unitCode);
      m_subTypeManager->toProtocolState(rbuf.THERMOSTAT3.cmnd);
      rbuf.THERMOSTAT3.signalPower = 0;
      rbuf.THERMOSTAT3.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT3));
   }

   void CThermostat3::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CThermostat3::getDeviceName() const
   {
      return m_deviceName;
   }

   void CThermostat3::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_unitCode;
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages


