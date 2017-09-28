#include "stdafx.h"
#include "Security1X10M.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

   CSecurity1X10M::CSecurity1X10M()
      : m_alarm(boost::make_shared<yApi::historization::CSwitch>("alarm", yApi::EKeywordAccessMode::kGet)),
      m_tamper(boost::make_shared<yApi::historization::CSwitch>("tamper", yApi::EKeywordAccessMode::kGet)),
      m_keywords({ m_alarm, m_tamper })
   {
   }

   CSecurity1X10M::~CSecurity1X10M()
   {
   }

   std::string CSecurity1X10M::getModel() const
   {
      return "X10 security motion sensor";
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CSecurity1X10M::keywords() const
   {
      return m_keywords;
   }

   void CSecurity1X10M::set(const std::string& /*keyword*/, const std::string& /*yadomsCommand*/)
   {
      // Get-only keywords
   }

   void CSecurity1X10M::reset()
   {
      m_alarm->set(false);
      m_tamper->set(false);
   }

   void CSecurity1X10M::setFromProtocolState(unsigned char statusByte)
   {
      m_alarm->set(statusByte == sStatusMotion || statusByte == sStatusMotionTamper);
      m_tamper->set(statusByte == sStatusNoMotionTamper || statusByte == sStatusMotionTamper);
   }

   unsigned char CSecurity1X10M::toProtocolState() const
   {
      // Get-only keywords
      return 0;
   }

   unsigned long CSecurity1X10M::idFromProtocol(const RBUF& rbuf) const
   {
      return static_cast<unsigned long>(rbuf.SECURITY1.id1);
   }

} // namespace rfxcomMessages