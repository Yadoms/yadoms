#include "stdafx.h"
#include "Cartelectronic.h"
#include <shared/exception/InvalidParameter.hpp>
#include "CartelectronicTIC.h"
#include "CartelectronicEncoder.h"
#include <shared/Log.h>
#include "CartelectronicLinky.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronic::CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const RBUF& rbuf,
                                    size_t rbufSize)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_keywords({m_signalPower, m_batteryLevel})
   {
      createSubType(api, rbuf.TIC.subtype, rbuf, rbufSize);

      m_id = m_subTypeManager->idFromProtocol(rbuf);
      m_batteryLevel->set(NormalizeBatteryLevel(m_subTypeManager->batteryLevelFromProtocol(rbuf)));
      m_signalPower->set(NormalizeSignalPowerLevel(m_subTypeManager->signalPowerFromProtocol(rbuf)));
      declare(api);
   }

   CCartelectronic::CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> api,
                                    unsigned char subType,
                                    const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& newDeviceConfiguration)
      : m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_keywords({m_signalPower, m_batteryLevel})
   {
      createSubType(subType, newDeviceConfiguration);

      // Remove obsolete keywords, add new keywords, don't touch existing keywords (to keep history)
      const auto existingKeywords = api->getAllKeywords(newDeviceConfiguration->name());
      auto newKeywords = m_subTypeManager->keywords(newDeviceConfiguration->configuration());
      // Never delete signalPower and batteryLevel
      newKeywords.push_back(m_signalPower);
      newKeywords.push_back(m_batteryLevel);

      // Remove obsolete keywords
      for (const auto existingKeyword : existingKeywords)
      {
         if (std::find_if(newKeywords.begin(),
                          newKeywords.end(),
                          [&existingKeyword](const boost::shared_ptr<const yApi::historization::IHistorizable>& newKeyword)
                          {
                             return newKeyword->getKeyword() == existingKeyword;
                          }) == newKeywords.end())
            api->removeKeyword(newDeviceConfiguration->name(), existingKeyword);
      }

      // Create new keywords
      for (const auto newKeyword : newKeywords)
      {
         if (std::find_if(existingKeywords.begin(),
                          existingKeywords.end(),
                          [&newKeyword](const std::string& existingKeyword)
                          {
                             return newKeyword->getKeyword() == existingKeyword;
                          }) == existingKeywords.end())
            api->declareKeyword(newDeviceConfiguration->name(), newKeyword);
      }
   }

   CCartelectronic::~CCartelectronic()
   {
   }

   void CCartelectronic::createSubType(boost::shared_ptr<yApi::IYPluginApi> api,
                                       unsigned char subType,
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
      case sTypeLinky: m_subTypeManager = boost::make_shared<CCartelectronicLinky>(rbuf, rbufSize);
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }

      const auto id = m_subTypeManager->idFromProtocol(rbuf);
      const auto subTypeKeywords = m_subTypeManager->keywords(api->deviceExists(id) ? api->getDeviceConfiguration(id) : new_CDataContainerSharedPtr());
      m_keywords.insert(m_keywords.end(),
                        subTypeKeywords.begin(),
                        subTypeKeywords.end());
   }

   void CCartelectronic::createSubType(unsigned char subType,
                                       const boost::shared_ptr<const yApi::ISetDeviceConfiguration>& deviceConfiguration)
   {
      m_subType = subType;

      switch (m_subType)
      {
      case sTypeCEencoder: m_subTypeManager = boost::make_shared<CCartelectronicEncoder>(deviceConfiguration);
         break;
      default:
         throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
      }
      const auto subTypeKeywords = m_subTypeManager->keywords(deviceConfiguration->configuration());
      m_keywords.insert(m_keywords.end(),
                        subTypeKeywords.begin(),
                        subTypeKeywords.end());
   }

   void CCartelectronic::declare(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      if (!m_subTypeManager)
         throw shared::exception::CException("m_subTypeManager must be initialized");

      // Create device and keywords if needed
      if (!api->deviceExists(m_id))
      {
         shared::CDataContainerSharedPtr details = new_CDataContainerSharedPtr();
         details->set("type", pTypeCARTELECTRONIC);
         details->set("subType", m_subType);
         details->set("id", m_id);

         const auto model = m_subTypeManager->getModel();
         api->declareDevice(m_id, model, model, m_keywords, details);
         YADOMS_LOG(information) << "New device : " << m_id << " (" << model << ")";
         details->printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CCartelectronic::encode(
      boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Cartelectronic is a read-only message, can not be encoded");
   }

   void CCartelectronic::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_id, m_keywords);
   }

   void CCartelectronic::filter() const
   {
   }

   void CCartelectronic::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CCartelectronic::getDeviceName() const
   {
      return m_id;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCartelectronic::keywords()
   {
      return m_keywords;
   }
} // namespace rfxcomMessages
