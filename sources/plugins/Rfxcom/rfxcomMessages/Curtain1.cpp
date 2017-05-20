#include "stdafx.h"
#include "Curtain1.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                        const std::string& command,
                        const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      m_state->setCommand(command);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_houseCode = deviceDetails.get<unsigned char>("houseCode");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      Init(api);
   }

   CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                        unsigned int subType,
                        const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_subType(0),
      m_houseCode(0),
      m_unitCode(0),
      m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      m_state->set(yApi::historization::ECurtainCommand::kStop);

      m_subType = static_cast<unsigned char>(subType);
      if (m_subType != sTypeHarrison)
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");

      m_houseCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration.get<char>("houseCode"));
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      Init(api);
   }

   CCurtain1::CCurtain1(boost::shared_ptr<yApi::IYPluginApi> api,
                        const RBUF& rbuf,
                        size_t rbufSize)
      : m_subType(0),
      m_houseCode(0),
      m_unitCode(0),
      m_state(boost::make_shared<yApi::historization::CCurtain>("state"))
   {
      // Should not be called (transmitter-only device)
      throw std::logic_error("Constructing Curtain1 object from received buffer is not possible, Curtain1 is transmitter-only device");
   }

   CCurtain1::~CCurtain1()
   {
   }

   void CCurtain1::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeCurtain);
         details.set("subType", m_subType);
         details.set("houseCode", m_houseCode);
         details.set("unitCode", m_unitCode);
         api->declareDevice(m_deviceName,
                            m_deviceModel,
                            m_deviceModel,
                            m_state,
                            details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCurtain1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF buffer;
      MEMCLEAR(buffer.CURTAIN1);

      buffer.CURTAIN1.packetlength = ENCODE_PACKET_LENGTH(CURTAIN1);
      buffer.CURTAIN1.packettype = pTypeCurtain;
      buffer.CURTAIN1.subtype = m_subType;
      buffer.CURTAIN1.seqnbr = seqNumberProvider->next();
      buffer.CURTAIN1.housecode = m_houseCode;
      buffer.CURTAIN1.unitcode = m_unitCode;
      buffer.CURTAIN1.cmnd = toProtocolState(*m_state);
      buffer.CURTAIN1.filler = 0;

      return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(CURTAIN1));
   }

   void CCurtain1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize (transmitter-only device)
   }

   const std::string& CCurtain1::getDeviceName() const
   {
      return m_deviceName;
   }

   void CCurtain1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<char>(m_houseCode) << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CCurtain1::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeHarrison: ssModel << "Harrison";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CCurtain1::toProtocolState(const yApi::historization::CCurtain& curtainState)
   {
      switch (curtainState.get()())
      {
      case yApi::historization::ECurtainCommand::kOpenValue: return curtain_sOpen;
      case yApi::historization::ECurtainCommand::kCloseValue: return curtain_sClose;
      case yApi::historization::ECurtainCommand::kStopValue: return curtain_sStop;
      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(curtainState.get()()));
      }
   }
} // namespace rfxcomMessages


