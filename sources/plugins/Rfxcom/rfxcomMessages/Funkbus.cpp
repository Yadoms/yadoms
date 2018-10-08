#include "stdafx.h"
#include "Funkbus.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFunkbus::CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& command,
                      const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      m_state->setCommand(command);
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_groupCode = deviceDetails.get<unsigned char>("groupCode");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");
      m_id = deviceDetails.get<unsigned short>("id");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CFunkbus::CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
                      unsigned int subType,
                      const std::string& name,
                      const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      m_state->set(false);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeFunkbusRemoteGira:
      case sTypeFunkbusRemoteInsta:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_groupCode = manuallyDeviceCreationConfiguration.get<char>("houseCode");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");
      m_id = manuallyDeviceCreationConfiguration.get<short>("id");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CFunkbus::CFunkbus(boost::shared_ptr<yApi::IYPluginApi> api,
                      const RBUF& rbuf,
                      size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeFunkbus,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(FUNKBUS),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.FUNKBUS.subtype;
      m_groupCode = rbuf.FUNKBUS.groupcode;
      m_unitCode = rbuf.FUNKBUS.unitcode;
      m_id = (rbuf.FUNKBUS.id1 << 8) | rbuf.FUNKBUS.id2;
      m_state->set(fromProtocolState(rbuf.FUNKBUS.cmnd));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.FUNKBUS.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CFunkbus::~CFunkbus()
   {
   }

   void CFunkbus::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeFunkbus);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("groupCode", m_groupCode);
         m_deviceDetails.set("unitCode", m_unitCode);
         m_deviceDetails.set("id", m_id);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CFunkbus::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.FUNKBUS);

      rbuf.FUNKBUS.packetlength = ENCODE_PACKET_LENGTH(FUNKBUS);
      rbuf.FUNKBUS.packettype = pTypeFunkbus;
      rbuf.FUNKBUS.subtype = m_subType;
      rbuf.FUNKBUS.seqnbr = seqNumberProvider->next();
      rbuf.FUNKBUS.id1 = static_cast<unsigned char>(m_id >> 8);
      rbuf.FUNKBUS.id2 = static_cast<unsigned char>(m_id);
      rbuf.FUNKBUS.groupcode = m_groupCode;
      rbuf.FUNKBUS.unitcode = m_unitCode;
      rbuf.FUNKBUS.cmnd = toProtocolState(*m_state);
      rbuf.FUNKBUS.cmndtime = 0;
      rbuf.FUNKBUS.rssi = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(FUNKBUS));
   }

   void CFunkbus::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CFunkbus::filter() const
   {
   }

   void CFunkbus::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails.printToLog(YADOMS_LOG(information));
   }

   const std::string& CFunkbus::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFunkbus::keywords()
   {
      return m_keywords;
   }

   void CFunkbus::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<char>(m_groupCode) << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CFunkbus::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeFunkbusRemoteGira: ssModel << "Gira";
         break;
      case sTypeFunkbusRemoteInsta: ssModel << "Insta";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CFunkbus::toProtocolState(const yApi::historization::CSwitch& switchState)
   {
      return switchState.get() ? Funkbus_sChannelPlus : Funkbus_sChannelMin;
   }

   bool CFunkbus::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case Funkbus_sChannelPlus:
      case Funkbus_sAllOn:
      case Funkbus_sMasterPlus:
         return true;
         
      case Funkbus_sChannelMin:
      case Funkbus_sAllOff:
      case Funkbus_sMasterMin:
         return false;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages
