#include "stdafx.h"
#include "MicrolanCoupler.h"
#include "Identification.h"

namespace device {
   
CMicrolanCoupler::CMicrolanCoupler(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ISingleSwitch> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2409")), m_context(context), m_io(io), m_state("state")
{
   BOOST_ASSERT_MSG(m_identification->family() == kMicrolanCoupler, "Invalid family number");
}

CMicrolanCoupler::~CMicrolanCoupler()
{
}

boost::shared_ptr<const IIdentification> CMicrolanCoupler::ident() const
{
   return m_identification;
}

void CMicrolanCoupler::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), m_state))
      m_context->declareKeyword(m_identification->deviceName(), m_state);
}

void CMicrolanCoupler::historize()
{
   m_state.set(m_io->read());
   m_context->historizeData(ident()->deviceName(), m_state);
}

void CMicrolanCoupler::set(const std::string& keyword, const std::string& command)
{
   if (m_state.getKeyword() != keyword)
      std::cerr << "Unknown keyword " << keyword << std::endl;

   if (m_state.getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
      std::cerr << "Try to drive the read-only keyword " << keyword << std::endl;

   m_state.setCommand(command);
   m_io->write(m_state.get());
}

} // namespace device
