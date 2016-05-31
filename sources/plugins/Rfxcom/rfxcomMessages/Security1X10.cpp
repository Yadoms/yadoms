#include "stdafx.h"
#include "Security1X10.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSecurity1X10::CSecurity1X10()
      : m_alarm(boost::make_shared<yApi::historization::CSwitch>("alarm", yApi::EKeywordAccessMode::kGet)),
        m_tamper(boost::make_shared<yApi::historization::CSwitch>("tamper", yApi::EKeywordAccessMode::kGet)),
        m_keywords({m_alarm, m_tamper})
   {
   }

   CSecurity1X10::~CSecurity1X10()
   {
   }

   std::string CSecurity1X10::getModel() const
   {
      return "X10 security door/window sensor";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1X10::keywords() const
   {
      return m_keywords;
   }

   void CSecurity1X10::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
   {
      // Get-only keywords
   }

   void CSecurity1X10::reset()
   {
      m_alarm->set(false);
      m_tamper->set(false);
   }

   void CSecurity1X10::setFromProtocolState(unsigned char statusByte)
   {
      m_alarm->set(statusByte == sStatusAlarm || statusByte == sStatusAlarmDelayed || statusByte == sStatusAlarmTamper || statusByte == sStatusAlarmDelayedTamper);
      m_tamper->set(statusByte == sStatusNormalTamper || statusByte == sStatusNormalDelayedTamper || statusByte == sStatusAlarmTamper || statusByte == sStatusAlarmDelayedTamper);
   }

   unsigned char CSecurity1X10::toProtocolState() const
   {
      // Get-only keywords
      return 0;
   }

   unsigned long CSecurity1X10::idFromProtocol(const RBUF& rbuf) const
   {
      return static_cast<unsigned long>(rbuf.SECURITY1.id1);
   }
} // namespace rfxcomMessages


