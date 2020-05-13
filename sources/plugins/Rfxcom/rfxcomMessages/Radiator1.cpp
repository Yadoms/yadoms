#include "stdafx.h"
#include "Radiator1.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& keyword,
                          const std::string& command,
                          const boost::shared_ptr<shared::CDataContainer>& deviceDetails)
      : m_day(boost::make_shared<yApi::historization::CSwitch>("day")),
        m_setPoint(boost::make_shared<yApi::historization::CTemperature>("setPoint")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_day , m_setPoint , m_signalPower})
   {
      if (boost::iequals(keyword, m_day->getKeyword()))
      {
         m_day->setCommand(command);
         m_dayNightCmd = true;
      }
      else if (boost::iequals(keyword, m_setPoint->getKeyword()))
      {
         m_setPoint->setCommand(command);
         m_dayNightCmd = false;
      }
      else
         throw shared::exception::CInvalidParameter(keyword);

      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(deviceDetails->get<unsigned int>("subType"));
      m_id = deviceDetails->get<unsigned int>("id");
      m_unitCode = static_cast<unsigned char>(deviceDetails->get<unsigned int>("unitCode"));

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                          unsigned int subType,
                          const std::string& name,
                          const boost::shared_ptr<shared::CDataContainer>& manuallyDeviceCreationConfiguration)
      : m_deviceName(name),
        m_dayNightCmd(false),
        m_day(boost::make_shared<yApi::historization::CSwitch>("day")),
        m_setPoint(boost::make_shared<yApi::historization::CTemperature>("setPoint")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_day , m_setPoint , m_signalPower})
   {
      m_day->set(false);
      m_setPoint->set(0.0);
      m_signalPower->set(0);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeSmartwares:
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      m_id = manuallyDeviceCreationConfiguration->get<unsigned int>("id");
      m_unitCode = static_cast<unsigned char>(manuallyDeviceCreationConfiguration->get<unsigned int>("unitCode"));

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CRadiator1::CRadiator1(boost::shared_ptr<yApi::IYPluginApi> api,
                          const RBUF& rbuf,
                          size_t rbufSize)
      : m_subType(0),
        m_unitCode(0),
        m_id(0),
        m_dayNightCmd(false),
        m_day(boost::make_shared<yApi::historization::CSwitch>("day")),
        m_setPoint(boost::make_shared<yApi::historization::CTemperature>("setPoint")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_day , m_setPoint , m_signalPower})
   {
      // Should not be called (transmitter-only device)
      throw std::logic_error("Constructing CRadiator1 object from received buffer is not possible, CRadiator1 is transmitter-only device");
   }

   CRadiator1::~CRadiator1()
   {
   }

   void CRadiator1::buildDeviceDetails()
   {
      if (m_deviceDetails->empty())
      {
         m_deviceDetails->set("type", pTypeRadiator1);
         m_deviceDetails->set("subType", m_subType);
         m_deviceDetails->set("id", m_id);
         m_deviceDetails->set("unitCode", m_unitCode);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CRadiator1::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF rbuf;
      MEMCLEAR(rbuf.RADIATOR1);

      rbuf.RADIATOR1.packetlength = ENCODE_PACKET_LENGTH(RADIATOR1);
      rbuf.RADIATOR1.packettype = pTypeRadiator1;
      rbuf.RADIATOR1.subtype = m_subType;
      rbuf.RADIATOR1.seqnbr = seqNumberProvider->next();
      rbuf.RADIATOR1.id1 = static_cast<unsigned char>((m_id & 0xFF000000) >> 24);
      rbuf.RADIATOR1.id2 = static_cast<unsigned char>((m_id & 0xFF0000) >> 16);
      rbuf.RADIATOR1.id3 = static_cast<unsigned char>((m_id & 0xFF00) >> 8);
      rbuf.RADIATOR1.id4 = static_cast<unsigned char>(m_id & 0xFF);
      rbuf.RADIATOR1.unitcode = m_unitCode;
      toProtocolState(rbuf);
      rbuf.RADIATOR1.rssi = 0;
      rbuf.RADIATOR1.filler = 0;

      return toBufferQueue(rbuf, GET_RBUF_STRUCT_SIZE(RADIATOR1));
   }

   void CRadiator1::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize (transmitter-only device)
   }

   void CRadiator1::filter() const
   {
   }

   void CRadiator1::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CRadiator1::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRadiator1::keywords()
   {
      return m_keywords;
   }

   void CRadiator1::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel <<
         "." << static_cast<unsigned int>(m_id) <<
         "." << static_cast<unsigned int>(m_unitCode);
      m_deviceName = ssdeviceName.str();
   }

   void CRadiator1::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeSmartwares: ssModel << "Smartwares radiator valve";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   void CRadiator1::toProtocolState(RBUF& radiator1) const
   {
      if (m_dayNightCmd)
      {
         radiator1.RADIATOR1.cmnd = m_day.get() ? Radiator1_sDay : Radiator1_sNight;
         radiator1.RADIATOR1.temperature = 0;
         radiator1.RADIATOR1.tempPoint5 = 0;
      }
      else
      {
         radiator1.RADIATOR1.cmnd = Radiator1_sSetTemp;
         // Round set point to nearest 0.5
         const double roundedSetPoint = floor(m_setPoint->get() * 2) / 2;
         double setPointIntegerPartAsFloat;
         const auto setPointDecimalPart = static_cast<int>(modf(roundedSetPoint, &setPointIntegerPartAsFloat));
         const auto setPointIntegerPart = static_cast<int>(setPointIntegerPartAsFloat);

         radiator1.RADIATOR1.temperature = static_cast<BYTE>(setPointIntegerPart);
         radiator1.RADIATOR1.tempPoint5 = setPointDecimalPart == 0 ? 0 : 5;
      }
   }
} // namespace rfxcomMessages


