#include "stdafx.h"
#include "AdressableSwitch.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {
   
CAdressableSwitch::CAdressableSwitch(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ISingleSwitch> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2405")), m_context(context), m_io(io), m_state("state")
{
   BOOST_ASSERT_MSG(m_identification->family() == kAddresableSwitch, "Invalid family number");
}

CAdressableSwitch::~CAdressableSwitch()
{
}

boost::shared_ptr<const IIdentification> CAdressableSwitch::ident() const
{
   return m_identification;
}

void CAdressableSwitch::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), m_state))
      m_context->declareKeyword(m_identification->deviceName(), m_state);
}

void CAdressableSwitch::historize()
{
   m_state.set(m_io->read());
   m_context->historizeData(ident()->deviceName(), m_state);
}

void CAdressableSwitch::set(const std::string& keyword, const std::string& command)
{
   if (m_state.getKeyword() != keyword)
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
   
   if (m_state.getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
   
   m_state.setCommand(command);
   m_io->write(m_state.get());
}

} // namespace device
