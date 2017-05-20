#include "stdafx.h"
#include "Lighting6.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting6::CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      m_state->setCommand(command);
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned short>("id");
      m_groupCode = deviceDetails.get<unsigned char>("groupCode");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      Init(api);
   }

   CLighting6::CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      m_state->set(false);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypeBlyss)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_id = manuallyDeviceCreationConfiguration.get<short>("id");
      m_groupCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration.get<char>("groupCode"));
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      Init(api);
   }

   CLighting6::CLighting6(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_state , m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting6,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING6),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.LIGHTING6.subtype;
      m_id = (rbuf.LIGHTING6.id1 << 8) | rbuf.LIGHTING6.id2;
      m_groupCode = rbuf.LIGHTING6.groupcode;
      m_unitCode = rbuf.LIGHTING6.unitcode;
      m_state->set(fromProtocolState(rbuf.LIGHTING6.cmnd));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING6.rssi));

      Init(api);
   }

   CLighting6::~CLighting6()
   {
   }

   void CLighting6::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeLighting6);
         details.set("subType", m_subType);
         details.set("id", m_id);
         details.set("groupCode", m_groupCode);
         details.set("unitCode", m_unitCode);

         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CLighting6::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.LIGHTING6);

      rbuf.LIGHTING6.packetlength = ENCODE_PACKET_LENGTH(LIGHTING6);
      rbuf.LIGHTING6.packettype = pTypeLighting6;
      rbuf.LIGHTING6.subtype = m_subType;
      rbuf.LIGHTING6.seqnbr = seqNumberProvider->next();
      rbuf.LIGHTING6.id1 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.LIGHTING6.id2 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.LIGHTING6.groupcode = m_groupCode;
      rbuf.LIGHTING6.unitcode = m_unitCode;
      rbuf.LIGHTING6.cmnd = toProtocolState(*m_state);
      rbuf.LIGHTING6.cmndseqnbr = seqNumberProvider->last() % 4;
      rbuf.LIGHTING6.seqnbr2 = 0;
      rbuf.LIGHTING6.rssi = 0;
      rbuf.LIGHTING6.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING6));
   }

   void CLighting6::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CLighting6::getDeviceName() const
   {
      return m_deviceName;
   }

   void CLighting6::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel <<
         "." << static_cast<char>(m_groupCode) <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CLighting6::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeBlyss: ssModel << "Blyss";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CLighting6::toProtocolState(const yApi::historization::CSwitch& switchState)
   {
      return switchState.get() ? light6_sOn : light6_sOff;
   }

   bool CLighting6::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case light6_sOn:
      case light6_sGroupOn:
         return true;

      case light6_sOff:
      case light6_sGroupOff:
         return false;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages


