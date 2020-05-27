#include "stdafx.h"
#include "AdressableSwitch.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device
{
const boost::posix_time::time_duration CAdressableSwitch::HistorizationPeriod(boost::posix_time::seconds(5));

CAdressableSwitch::CAdressableSwitch(EOneWireFamily family,
                                     const std::string &id,
                                     boost::shared_ptr<ioInterfaces::ISingleSwitch> io)
    : m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2405")),
      m_io(io),
      m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
      m_keywords({m_state})
{
   BOOST_ASSERT_MSG(m_identification->family() == kAddresableSwitch, "Invalid family number");
}

void CAdressableSwitch::setConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                         const shared::CDataContainer &configuration)
{
   YADOMS_LOG(error) << "Try to apply a device configuration to an unconfigurable device";
}

void CAdressableSwitch::read() const
{
   m_state->set(m_io->read());
}

void CAdressableSwitch::write(const std::string &keyword, const std::string &command)
{
   if (m_state->getKeyword() != keyword)
      YADOMS_LOG(error) << "Unknown keyword " << keyword;

   if (m_state->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;

   m_state->setCommand(command);
   m_io->write(m_state->get());
}

bool CAdressableSwitch::hasRelevantValue()
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
