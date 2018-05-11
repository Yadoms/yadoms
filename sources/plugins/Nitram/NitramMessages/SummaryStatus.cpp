#include "stdafx.h"
#include "SummaryStatus.h"
#include <shared/Log.h>
#include "MessagesDefinition.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   CSummaryStatus::CSummaryStatus(std::string deviceName)
      : m_deviceName(deviceName), 
      m_ACPresent(boost::make_shared<yApi::historization::CSwitch>("ACPresent", yApi::EKeywordAccessMode::kGet)),
      m_UPSStatus(boost::make_shared<nitram::specificHistorizers::CStatus>("Status")),
      m_lowRuntime(boost::make_shared<yApi::historization::CSwitch>("lowRuntime", yApi::EKeywordAccessMode::kGet)),
      m_lowCapacity(boost::make_shared<yApi::historization::CSwitch>("lowCapacity", yApi::EKeywordAccessMode::kGet)),
      m_keywords({ m_ACPresent, m_UPSStatus, m_lowRuntime , m_lowCapacity })
   {}

   void CSummaryStatus::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                               const shared::communication::CByteBuffer& buffer)
   {
      nitram::specificHistorizers::EUPSState tempStatus;
      static bool firstRun = true;

      m_keywords.clear();
      if (((buffer[2] & 0x01) != m_ACPresent->get()) || firstRun)
      {
         m_ACPresent->set((buffer[2] & 0x01) != 0);
         YADOMS_LOG(information) << "AC Present : " << m_ACPresent->get();
         m_keywords.push_back(m_ACPresent);
      }

      if (((buffer[2] & 0x10) != m_lowRuntime->get()) || firstRun)
      {
         m_lowRuntime->set((buffer[2] & 0x10) != 0);
         YADOMS_LOG(information) << "Low Runtime : " << m_lowRuntime->get();
         m_keywords.push_back(m_lowRuntime);
      }

      if (((buffer[2] & 0x20) != m_lowCapacity->get()) || firstRun)
      {
         m_lowCapacity->set((buffer[2] & 0x20) != 0);
         YADOMS_LOG(information) << "Low Capacity : " << m_lowCapacity->get();
         m_keywords.push_back(m_lowCapacity);
      }

      if (buffer[2] & 0x02)
         tempStatus = nitram::specificHistorizers::EUPSState::kCharging;
      else if (buffer[2] & 0x04)
         tempStatus = nitram::specificHistorizers::EUPSState::kDischarging;
      else if (buffer[2] & 0x08)
         tempStatus = nitram::specificHistorizers::EUPSState::kFullyCharged;

      if ((tempStatus != m_UPSStatus->get()) || firstRun)
      {
         m_UPSStatus->set(tempStatus);
         YADOMS_LOG(information) << "UPS Status : " << m_UPSStatus->get();
         m_keywords.push_back(m_UPSStatus);
      }

      firstRun = false;
   }

   CSummaryStatus::~CSummaryStatus()
   {}

   bool CSummaryStatus::getACPresent()
   {
      return m_ACPresent->get();
   }

   boost::shared_ptr<shared::communication::CByteBuffer> CSummaryStatus::encode() const
   {
      shared::communication::CByteBuffer message(1);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::SummaryStatus;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CSummaryStatus::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CSummaryStatus::keywords() const
   {
      return m_keywords;
   }

   bool CSummaryStatus::onlyAck() const
   {
      return false;
   }

} // namespace nitramMessages