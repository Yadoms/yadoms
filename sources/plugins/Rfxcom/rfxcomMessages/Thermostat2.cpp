#include "stdafx.h"
#include "Thermostat2.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& command,
                              const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_deviceDetails(shared::CDataContainer::make()),
        m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_signalPower})
   {
      m_state->setCommand(command);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_unitCode = deviceDetails->get<unsigned int>("unitCode");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                              unsigned int subType,
                              const std::string& name,
                              const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_deviceDetails(shared::CDataContainer::make()),
        m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_signalPower})
   {
      m_state->set(false);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeHE105:
      case sTypeRTS10:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_unitCode = manuallyDeviceCreationConfiguration->get<unsigned int>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CThermostat2::CThermostat2(boost::shared_ptr<yApi::IYPluginApi> api,
                              const RBUF& rbuf,
                              size_t rbufSize)
      : m_deviceDetails(shared::CDataContainer::make()),
        m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state, m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeThermostat2,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(THERMOSTAT2),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.THERMOSTAT2.subtype;
      m_unitCode = rbuf.THERMOSTAT2.unitcode;
      m_state->set(fromProtocolState(rbuf.THERMOSTAT2.cmnd));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.THERMOSTAT2.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CThermostat2::~CThermostat2()
   {
   }

   void CThermostat2::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeThermostat2);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CThermostat2::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.THERMOSTAT2);

      rbuf.THERMOSTAT2.packetlength = ENCODE_PACKET_LENGTH(THERMOSTAT2);
      rbuf.THERMOSTAT2.packettype = pTypeThermostat2;
      rbuf.THERMOSTAT2.subtype = m_subType;
      rbuf.THERMOSTAT2.seqnbr = seqNumberProvider->next();
      rbuf.THERMOSTAT2.unitcode = static_cast<BYTE>(m_unitCode);
      rbuf.THERMOSTAT2.cmnd = toProtocolState(*m_state);
      rbuf.THERMOSTAT2.rssi = 0;
      rbuf.THERMOSTAT2.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(THERMOSTAT2));
   }

   void CThermostat2::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CThermostat2::filter() const
   {
   }

   void CThermostat2::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails->printToLog(YADOMS_LOG(information));
   }

   const std::string& CThermostat2::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CThermostat2::keywords()
   {
      return m_keywords;
   }

   void CThermostat2::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CThermostat2::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeHE105: ssModel << "HE105";
         break;
      case sTypeRTS10: ssModel << "RTS10, RFS10, TLX1206";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CThermostat2::toProtocolState(const yApi::historization::CSwitch& switchState)
   {
      return switchState.get() ? thermostat2_sOn : thermostat2_sOff;
   }

   bool CThermostat2::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case thermostat2_sOn: return true;
      case thermostat2_sOff: return false;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages
