#include "stdafx.h"
#include "AdressableSwitch.h"
#include "Identification.h"

namespace device
{
   CAdressableSwitch::CAdressableSwitch(EOneWireFamily family,
                                        const std::string& id,
                                        boost::shared_ptr<ioInterfaces::ISingleSwitch> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2405")),
       m_io(io),
       m_state(boost::make_shared<yApi::historization::CSwitch>("state")),
       m_keywords({ m_state })
   {
      BOOST_ASSERT_MSG(m_identification->family() == kAddresableSwitch, "Invalid family number");
   }

   CAdressableSwitch::~CAdressableSwitch()
   {
   }

   void CAdressableSwitch::read() const
   {
      m_state->set(m_io->read());
   }

   void CAdressableSwitch::write(const std::string& keyword, const std::string& command)
   {
      if (m_state->getKeyword() != keyword)
         std::cerr << "Unknown keyword " << keyword << std::endl;

      if (m_state->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
         std::cerr << "Try to drive the read-only keyword " << keyword << std::endl;

      m_state->setCommand(command);
      m_io->write(m_state->get());
   }
} // namespace device
