#include "stdafx.h"
#include "SetupStartupDelay.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "MessagesDefinition.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   CSetupStartupDelay::CSetupStartupDelay():
      m_delay(0)
   {}

   CSetupStartupDelay::~CSetupStartupDelay()
   {}

   boost::shared_ptr<shared::communication::CByteBuffer> CSetupStartupDelay::encode() const
   {
      shared::communication::CByteBuffer message(3);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::SetupStartupDelay;

      YADOMS_LOG(information) << "set Startup delay : " << m_delay;

      //The delay value
      message[1] = (unsigned char)(m_delay & 0xFF);
      message[2] = (unsigned char)(m_delay >> 8) & 0xFF;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CSetupStartupDelay::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                                   const shared::communication::CByteBuffer& buffer)
   {
      // Nothing to decode
   }

   void CSetupStartupDelay::setMessageForShutdown()
   {
      m_delay = 65535;
   }

   void CSetupStartupDelay::setDelay(boost::posix_time::time_duration delay)
   {
      m_delay = static_cast<unsigned int>(delay.minutes());
   }

   void CSetupStartupDelay::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Nothing to historize
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CSetupStartupDelay::keywords() const
   {
      return NoKeywords;
   }

   bool CSetupStartupDelay::onlyAck() const
   {
      return true;
   }
} // namespace rfxcomMessages