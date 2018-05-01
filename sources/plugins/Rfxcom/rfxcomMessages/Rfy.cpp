#include "stdafx.h"
#include "Rfy.h"
#include <shared/plugin/yPluginApi/historization/Curtain.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& command,
              const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_keywords{m_state}
   {
      m_state->setCommand(command);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");
      
      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              unsigned int subType,
              const std::string& name,
              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_keywords{m_state}
   {
      m_state->set(yApi::historization::ECurtainCommand::kStop);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeRFY:
      case sTypeRFYext:
      case sTypeASA:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeyword(m_deviceName, m_state);
   }

   CRfy::CRfy(boost::shared_ptr<yApi::IYPluginApi> api,
              const RBUF& rbuf,
              size_t rbufSize)
      : m_subType(0),
        m_unitCode(0),
        m_id(0),
        m_state(boost::make_shared<yApi::historization::CCurtain>("state")),
        m_keywords{m_state}
   {
      // Should not be called (transmitter-only device)
      throw std::logic_error("Constructing CRfy object from received buffer is not possible, CRfy is transmitter-only device");
   }

   CRfy::~CRfy()
   {
   }

   void CRfy::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeRFY);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
         m_deviceDetails.set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CRfy::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.RFY);

      rbuf.RFY.packetlength = ENCODE_PACKET_LENGTH(RFY);
      rbuf.RFY.packettype = pTypeRFY;
      rbuf.RFY.subtype = m_subType;
      rbuf.RFY.seqnbr = seqNumberProvider->next();
      rbuf.RFY.id1 = static_cast<unsigned char>((m_id & 0xFF0000) >> 16);
      rbuf.RFY.id2 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.RFY.id3 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.RFY.unitcode = m_unitCode;
      rbuf.RFY.cmnd = toProtocolState(*m_state);
      rbuf.RFY.rfu1 = 0;
      rbuf.RFY.rfu2 = 0;
      rbuf.RFY.rfu3 = 0;
      rbuf.RFY.rssi = 0;
      rbuf.RFY.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(RFY));
   }

   void CRfy::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize (transmitter-only device)
   }

   void CRfy::filter() const
   {
   }

   void CRfy::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CRfy::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRfy::keywords()
   {
      return m_keywords;
   }

   void CRfy::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CRfy::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeRFY: ssModel << "Somfy";
         break;
      case sTypeRFYext: ssModel << "SomfyExt";
         break;
      case sTypeASA: ssModel << "ASA";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CRfy::toProtocolState(const yApi::historization::CCurtain& curtainState)
   {
      switch (curtainState.get()())
      {
      case yApi::historization::ECurtainCommand::kOpenValue: return rfy_sUp;
      case yApi::historization::ECurtainCommand::kCloseValue: return rfy_sDown;
      case yApi::historization::ECurtainCommand::kStopValue: return rfy_sStop;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(curtainState.get()()));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CBuffer<unsigned char>>> CRfy::encodeProgramMessage(boost::shared_ptr<ISequenceNumber> seqNumberProvider,
                                                                                                           const shared::communication::CByteBuffer& lastRequest)
   {
      const auto lastRfyRequest = &reinterpret_cast<const RBUF* const>(lastRequest.begin())->RFY;

      RBUF rbuf;
      MEMCLEAR(rbuf.RFY);

      rbuf.RFY.packetlength = ENCODE_PACKET_LENGTH(RFY);
      rbuf.RFY.packettype = pTypeRFY;
      rbuf.RFY.subtype = lastRfyRequest->subtype;
      rbuf.RFY.seqnbr = seqNumberProvider->next();
      rbuf.RFY.id1 = lastRfyRequest->id1;
      rbuf.RFY.id2 = lastRfyRequest->id2;
      rbuf.RFY.id3 = lastRfyRequest->id3;
      rbuf.RFY.unitcode = lastRfyRequest->unitcode;
      rbuf.RFY.cmnd = rfy_sProgram;
      rbuf.RFY.rfu1 = 0;
      rbuf.RFY.rfu2 = 0;
      rbuf.RFY.rfu3 = 0;
      rbuf.RFY.rssi = 0;
      rbuf.RFY.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(RFY));
   }
} // namespace rfxcomMessages
