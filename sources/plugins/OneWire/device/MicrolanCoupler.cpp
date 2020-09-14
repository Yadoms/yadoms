#include "stdafx.h"
#include "MicrolanCoupler.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration CMicrolanCoupler::HistorizationPeriod(boost::posix_time::seconds(5));

CMicrolanCoupler::CMicrolanCoupler(EOneWireFamily family,
                                   const std::string &id,
                                   boost::shared_ptr<ioInterfaces::ISingleSwitch> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2409")),
      m_io(io),
      m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_keywords({m_state})
{
   BOOST_ASSERT_MSG(m_identification->family() == kMicrolanCoupler, "Invalid family number");
}

void CMicrolanCoupler::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                        const boost::shared_ptr<shared::CDataContainer> &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void CMicrolanCoupler::read() const
{
   m_state->set(m_io->read());
}

void CMicrolanCoupler::write(const std::string &keyword, const std::string &command)
{
   if (m_state->getKeyword() != keyword)
      YADOMS_LOG(error) << "Unknown keyword " << keyword;

   if (m_state->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;

   m_state->setCommand(command);
   m_io->write(m_state->get());
}

bool CMicrolanCoupler::hasRelevantValue()
{
   if (m_keywords.empty())
      return false;

   const auto now = shared::currentTime::Provider().now();

   if (now < (m_lastHistorizationDate + HistorizationPeriod))
      return false;

   m_lastHistorizationDate = now;
   return true;
}
} // namespace device
