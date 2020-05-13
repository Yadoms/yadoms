#include "stdafx.h"
#include "Thermostat3.h"
#include "Thermostat3MertikG6RHxT1.h"
#include "Thermostat3MertikG6RH4TB.h"
#include "Thermostat3MertikG6RH4TD.h"
#include "Thermostat3MertikG6RH4S.h"
#include <shared/Log.h>
// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& keyword,
                              const std::string& command,
                              const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_unitCode = deviceDetails->get<unsigned int>("unitCode");

      Init(api);
      m_subTypeManager->set(keyword, command);
   }

   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const std::string& name,
                              const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
   {
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);

      m_unitCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration->get<unsigned int>("unitCode"));

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->reset();
   }

   CThermostat3::CThermostat3(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_signalPower})
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
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.THERMOSTAT3.rssi));

      Init(api);
   }

   CThermostat3::~CThermostat3()
   {
   }

   void CThermostat3::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeThermostat3);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("unitCode", m_unitCode);
      }
   }

   void CThermostat3::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      switch (m_subType)
      {
      case sTypeMertikG6RH4T1:
      case sTypeMertikG6RH3T1:
         m_subTypeManager = boost::make_shared<CThermostat3MertikG6RHxT1>(m_subType);
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
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CThermostat3::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
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
      rbuf.THERMOSTAT3.rssi = 0;
      rbuf.THERMOSTAT3.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT3));
   }

   void CThermostat3::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CThermostat3::filter() const
   {
   }

   void CThermostat3::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      auto model = m_subTypeManager->getModel();
      api->declareDevice(m_deviceName, model, model, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << model << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CThermostat3::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CThermostat3::keywords()
   {
      return m_keywords;
   }

   void CThermostat3::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_unitCode;
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
