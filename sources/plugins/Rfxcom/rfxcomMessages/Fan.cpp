#include "stdafx.h"
#include "Fan.h"
#include <shared/Log.h>
#include "FanFt1211R.h"
#include "FanFalmec.h"
#include "FanItho.h"
#include "FanLucciAirDc.h"
#include "FanLucciAirDc2.h"
#include "FanLucciAirWestinghouseCasafan.h"
#include "FanSeavTXS4.h"
#include "FanSiemensSF01.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> api,
              const std::string& keyword,
              const std::string& command,
              const shared::CDataContainer& deviceDetails)
   {
      createSubType(deviceDetails.get<unsigned char>("subType"));
      m_subTypeManager->set(keyword, command);
      m_id = deviceDetails.get<unsigned int>("id");

      // Build device description
      buildDeviceName();
      m_deviceDetails = deviceDetails;
   }

   CFan::CFan(boost::shared_ptr<yApi::IYPluginApi> api,
              unsigned int subType,
              const std::string& name,
              const shared::CDataContainer& manuallyDeviceCreationConfiguration)
      : m_deviceName(name)
   {
      createSubType(static_cast<unsigned char>(subType));
      m_id = m_subTypeManager->idFromConfiguration(manuallyDeviceCreationConfiguration);

      buildDeviceDetails();
      api->updateDeviceDetails(m_deviceName, m_deviceDetails);
      api->declareKeywords(m_deviceName, m_keywords);

      m_subTypeManager->reset();
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

   void CFan::createSubType(unsigned char subType)
   {
      m_subType = subType;
      switch (m_subType)
      {
      case CFanSiemensSf01::kRfxValue: m_subTypeManager = boost::make_shared<CFanSiemensSf01>();
         break;
      case CFanItho::kRfxValue: m_subTypeManager = boost::make_shared<CFanItho>();
         break;
      case CFanLucciAirWestinghouseCasafan::kRfxValueLucciAir: m_subTypeManager = boost::make_shared<CFanLucciAirWestinghouseCasafan>(m_subType);
         break;
      case CFanLucciAirWestinghouseCasafan::kRfxValueWestinghouse: m_subTypeManager = boost::make_shared<CFanLucciAirWestinghouseCasafan>(m_subType);
         break;
      case CFanLucciAirWestinghouseCasafan::kRfxValueCasafan: m_subTypeManager = boost::make_shared<CFanLucciAirWestinghouseCasafan>(m_subType);
         break;
      case CFanSeavTxs4::kRfxValue: m_subTypeManager = boost::make_shared<CFanSeavTxs4>();
         break;
      case CFanLucciAirDc::kRfxValue: m_subTypeManager = boost::make_shared<CFanLucciAirDc>();
         break;
      case CFanFt1211R::kRfxValue: m_subTypeManager = boost::make_shared<CFanFt1211R>();
         break;
      case CFanFalmec::kRfxValue: m_subTypeManager = boost::make_shared<CFanFalmec>();
         break;
      case CFanLucciAirDc2::kRfxValue: m_subTypeManager = boost::make_shared<CFanLucciAirDc2>();
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      m_keywords.insert(m_keywords.end(), m_subTypeManager->keywords().begin(), m_subTypeManager->keywords().end());
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
      buffer.FAN.cmnd = m_subTypeManager->toProtocolCmnd();
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
      ssdeviceName << m_subTypeManager->getModel() << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }
} // namespace rfxcomMessages
