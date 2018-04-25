#include "stdafx.h"
#include "Fan.h"
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& keyword,
              const std::string& command,
              const shared::CDataContainer& deviceDetails)
      : m_cmd(keyword),
        m_light(boost::make_shared<yApi::historization::CSwitch>("light")),
        m_fan(boost::make_shared<yApi::historization::CSwitch>("fan")),
        m_t1(boost::make_shared<yApi::historization::CEvent>("t1")),
        m_t2(boost::make_shared<yApi::historization::CEvent>("t2")),
        m_t3(boost::make_shared<yApi::historization::CEvent>("t3")),
        m_t4(boost::make_shared<yApi::historization::CEvent>("t4"))
   {
      if (boost::iequals(keyword, m_light->getKeyword()))
      {
         m_keywords.push_back(m_light);
         m_light->setCommand(command);
      }
      else if (boost::iequals(keyword, m_fan->getKeyword()))
      {
         m_keywords.push_back(m_fan);
         m_fan->setCommand(command);
      }
      else
      {
         m_keywords.push_back(m_t1);
         m_keywords.push_back(m_t2);
         m_keywords.push_back(m_t3);
         m_keywords.push_back(m_t4);
      }

      m_subType = deviceDetails.get<unsigned char>("subType");
      m_id = deviceDetails.get<unsigned int>("id");

      // Build device description
      buildDeviceModel();
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> api,
              unsigned int subType,
              const std::string& name,
              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name)
   {
      m_light->set(false);
      m_fan->set(false);

      m_subType = static_cast<unsigned char>(subType);
      switch (m_subType)
      {
      case sTypeSiemensSF01:
      case sTypeLucciAir:
      case sTypeWestinghouse:
         m_light = boost::make_shared<yApi::historization::CSwitch>("light");
         m_fan = boost::make_shared<yApi::historization::CSwitch>("fan");
         m_keywords.push_back(m_light);
         m_keywords.push_back(m_fan);
         m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id");
         break;
      case sTypeSeavTXS4:
         m_t1 = boost::make_shared<yApi::historization::CEvent>("t1");
         m_t2 = boost::make_shared<yApi::historization::CEvent>("t2");
         m_t3 = boost::make_shared<yApi::historization::CEvent>("t3");
         m_t4 = boost::make_shared<yApi::historization::CEvent>("t4");
         m_keywords.push_back(m_t1);
         m_keywords.push_back(m_t2);
         m_keywords.push_back(m_t3);
         m_keywords.push_back(m_t4);
         m_id = manuallyDeviceCreationConfiguration.get<unsigned int>("id") |
         ((manuallyDeviceCreationConfiguration.get<bool>("sw2-1") ? 0x80 : 0x00) |
            (manuallyDeviceCreationConfiguration.get<bool>("sw2-2") ? 0x40 : 0x00) << 16);
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);
   }

   CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> api,
              const RBUF& rbuf,
              size_t rbufSize)
      : m_subType(0),
        m_id(0)
   {
      // Should not be called (transmitter-only device)
      throw std::logic_error("Constructing CFan object from received buffer is not possible, Cfan is transmitter-only device");
   }

   CFan::~CFan()
   {
   }

   void CFan::buildDeviceDetails()
   {
      if (m_deviceDetails.empty())
      {
         m_deviceDetails.set("type", pTypeFan);
         m_deviceDetails.set("subType", m_subType);
         m_deviceDetails.set("id", m_id);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CFan::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      RBUF buffer;
      MEMCLEAR(buffer.FAN);

      buffer.FAN.packetlength = ENCODE_PACKET_LENGTH(FAN);
      buffer.FAN.packettype = pTypeFan;
      buffer.FAN.subtype = m_subType;
      buffer.FAN.seqnbr = seqNumberProvider->next();
      buffer.FAN.id1 = static_cast<unsigned char>(0xFF & (m_id >> 16));
      buffer.FAN.id2 = static_cast<unsigned char>(0xFF & (m_id >> 8));
      buffer.FAN.id3 = static_cast<unsigned char>(0xFF & m_id);
      buffer.FAN.cmnd = toProtocolState();
      buffer.FAN.filler = 0;

      return toBufferQueue(buffer, GET_RBUF_STRUCT_SIZE(FAN));
   }

   void CFan::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      // Nothing to historize (transmitter-only device)
   }

   void CFan::filter() const
   {
   }

   void CFan::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CFan::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CFan::keywords()
   {
      return m_keywords;
   }

   void CFan::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << m_deviceModel << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CFan::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeSiemensSF01: ssModel << "Siemens SF01";
         break;
      case sTypeLucciAir: ssModel << "Lucci Air fan";
         break;
      case sTypeSeavTXS4: ssModel << "SEAV TXS4";
         break;
      case sTypeWestinghouse: ssModel << "Westinghouse 7226640";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }

   unsigned char CFan::toProtocolState() const
   {
      switch (m_subType)
      {
      case sTypeSiemensSF01:
         if (m_cmd == m_light->getKeyword())
            return m_light.get() ? fan_sLight : 0x00;
         return m_fan.get() ? fan_sPlus : fan_sMin;

      case sTypeLucciAir:
      case sTypeWestinghouse:
         if (m_cmd == m_light->getKeyword())
            return m_light.get() ? fan_LucciLight : 0x00;
         return m_fan.get() ? fan_LucciMed : fan_LucciOff;

      case sTypeSeavTXS4:
         if (m_cmd == m_t1->getKeyword())
            return fan_T1;
         if (m_cmd == m_t2->getKeyword())
            return fan_T2;
         if (m_cmd == m_t3->getKeyword())
            return fan_T3;
         if (m_cmd == m_t4->getKeyword())
            return fan_T4;
         return 0;

      default:
         throw shared::exception::COutOfRange("CFan : command to unsupported device");
      }
   }
} // namespace rfxcomMessages
