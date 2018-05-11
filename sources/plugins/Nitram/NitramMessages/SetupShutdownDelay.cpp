#include "stdafx.h"
#include "SetupShutdownDelay.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "MessagesDefinition.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   CSetupShutdownDelay::CSetupShutdownDelay() :
      m_delay(0) // Default delay in minutes
   {}

   CSetupShutdownDelay::~CSetupShutdownDelay()
   {}

   void CSetupShutdownDelay::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const shared::communication::CByteBuffer& buffer)
   {}

   boost::shared_ptr<shared::communication::CByteBuffer> CSetupShutdownDelay::encode() const
   {
      shared::communication::CByteBuffer message(3);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::SetupShutdownDelay;

      YADOMS_LOG(information) << "set Shutdown delay : " << m_delay;

      //The delay value
      message[1] = (unsigned char)(m_delay & 0xFF);
      message[2] = (unsigned char)(m_delay >> 8) & 0xFF;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CSetupShutdownDelay::setDelay(boost::posix_time::time_duration delay)
   {
      m_delay = static_cast<unsigned int>(delay.minutes());
   }

   void CSetupShutdownDelay::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Nothing to historize
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CSetupShutdownDelay::keywords() const
   {
      return NoKeywords;
   }

   bool CSetupShutdownDelay::onlyAck() const
   {
      return true;
   }
} // namespace nitramMessages