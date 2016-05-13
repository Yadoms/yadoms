#include "stdafx.h"
#include "Cartelectronic.h"
#include <shared/exception/InvalidParameter.hpp>
#include "CartelectronicTIC.h"
#include "CartelectronicEncoder.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronic::CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const RBUF& rbuf,
                                    size_t rbufSize,
                                    boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider):
      m_rssi(new yApi::historization::CRssi("rssi")),
      m_batteryLevel(new yApi::historization::CBatteryLevel("battery"))
   {
      createSubType(rbuf.TIC.subtype, rbuf, rbufSize);
      m_id = m_subTypeManager->idFromProtocol(rbuf);
      m_batteryLevel->set(NormalizeBatteryLevel(m_subTypeManager->BatteryLevelFromProtocol(rbuf)));
      m_rssi->set(NormalizeRssiLevel(m_subTypeManager->RssiFromProtocol(rbuf)));
      declare(api);
   }

   CCartelectronic::~CCartelectronic()
   {
   }

   void CCartelectronic::createSubType(unsigned char subType,
                                       const RBUF& rbuf,
                                       size_t rbufSize)
   {
      m_subType = subType;

      switch (m_subType)
      {
      case sTypeTIC: m_subTypeManager = boost::make_shared<CCartelectronicTIC>(rbuf, rbufSize);
         break;
      case sTypeCEencoder: m_subTypeManager = boost::make_shared<CCartelectronicEncoder>(rbuf, rbufSize);
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
   }

   void CCartelectronic::declare(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Create device and keywords if needed
      if (!api->deviceExists(m_id))
      {
         shared::CDataContainer details;
         details.set("type", pTypeCARTELECTRONIC);
         details.set("subType", m_subType);
         details.set("id", m_id);

         api->declareDevice(m_id, m_subTypeManager->getModel(), details);

         m_subTypeManager->declare(api, m_id);
         api->declareKeyword(m_id, *m_batteryLevel);
         api->declareKeyword(m_id, *m_rssi);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCartelectronic::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Cartelectronic is a read-only message, can not be encoded");
   }

   void CCartelectronic::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      std::vector<boost::shared_ptr<yApi::historization::IHistorizable>> KeywordList;

      KeywordList.push_back(m_batteryLevel);
      KeywordList.push_back(m_rssi);

      m_subTypeManager->historize(KeywordList);

      api->historizeData(m_id, KeywordList);
   }

   const std::string& CCartelectronic::getDeviceName() const
   {
      return m_id;
   }
} // namespace rfxcomMessages


