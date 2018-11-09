#include "stdafx.h"
#include "Lighting1.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CLighting1::CLighting1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& command,
                          const shared::CDataContainer& deviceDetails)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      m_state->setCommand(command);
      m_signalPower->set(0);

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_houseCode = deviceDetails.get<unsigned char>("houseCode");
      m_unitCode = deviceDetails.get<unsigned char>("unitCode");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CLighting1::CLighting1(boost::shared_ptr<yApi::IYPluginApi> api,
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
      case sTypeX10:
      case sTypeARC:
      case sTypeAB400D:
      case sTypeWaveman:
      case sTypeEMW200:
      case sTypeIMPULS:
      case sTypeRisingSun:
      case sTypePhilips:
      case sTypeEnergenie:
      case sTypeEnergenie5:
      case sTypeGDR2:
      case sTypeHQ:
      case sTypeOase:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_houseCode = manuallyDeviceCreationConfiguration.get<char>("houseCode");
      m_unitCode = manuallyDeviceCreationConfiguration.get<unsigned char>("unitCode");

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CLighting1::CLighting1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_state , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeLighting1,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(LIGHTING1),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.LIGHTING1.subtype;
      m_houseCode = rbuf.LIGHTING1.housecode;
      m_unitCode = rbuf.LIGHTING1.unitcode;
      m_state->set(fromProtocolState(rbuf.LIGHTING1.cmnd));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.LIGHTING1.rssi));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      buildDeviceDetails();
   }

   CLighting1::~CLighting1()
   {
   }

   void CLighting1::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeLighting1);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("houseCode", m_houseCode);
         m_deviceDetails.set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CLighting1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.LIGHTING1);

      rbuf.LIGHTING1.packetlength = ENCODE_PACKET_LENGTH(LIGHTING1);
      rbuf.LIGHTING1.packettype = pTypeLighting1;
      rbuf.LIGHTING1.subtype = m_subType;
      rbuf.LIGHTING1.seqnbr = seqNumberProvider->next();
      rbuf.LIGHTING1.housecode = m_houseCode;
      rbuf.LIGHTING1.unitcode = m_unitCode;
      rbuf.LIGHTING1.cmnd = toProtocolState(*m_state);
      rbuf.LIGHTING1.rssi = 0;
      rbuf.LIGHTING1.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(LIGHTING1));
   }

   void CLighting1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CLighting1::filter() const
   {
   }

   void CLighting1::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, m_deviceDetails);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      m_deviceDetails.printToLog(YADOMS_LOG(information));
   }

   const std::string& CLighting1::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CLighting1::keywords()
   {
      return m_keywords;
   }

   void CLighting1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<char>(m_houseCode) << "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CLighting1::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeX10: ssModel << "X10 lighting";
         break;
      case sTypeARC: ssModel << "ARC";
         break;
      case sTypeAB400D: ssModel << "ELRO AB400D (Flamingo)";
         break;
      case sTypeWaveman: ssModel << "Waveman";
         break;
      case sTypeEMW200: ssModel << "Chacon EMW200";
         break;
      case sTypeIMPULS: ssModel << "IMPULS";
         break;
      case sTypeRisingSun: ssModel << "RisingSun";
         break;
      case sTypePhilips: ssModel << "Philips SBC";
         break;
      case sTypeEnergenie: ssModel << "Energenie ENER010";
         break;
      case sTypeEnergenie5: ssModel << "Energenie 5-gang";
         break;
      case sTypeGDR2: ssModel << "COCO GDR2-2000R";
         break;
      case sTypeHQ: ssModel << "HQ COCO-20";
         break;
      case sTypeOase: ssModel << "Oase Inscenio FM Master";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CLighting1::toProtocolState(const yApi::historization::CSwitch& switchState)
   {
      return switchState.get() ? light1_sOn : light1_sOff;
   }

   bool CLighting1::fromProtocolState(unsigned char protocolState)
   {
      switch (protocolState)
      {
      case light1_sOn:
      case light1_sAllOn:
         return true;

      case light1_sOff:
      case light1_sAllOff:
         return false;

      default:
         throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
      }
   }
} // namespace rfxcomMessages
