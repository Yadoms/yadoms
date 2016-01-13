#include "stdafx.h"
#include "4kEepromWithPio.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {
   
C4kEepromWithPio::C4kEepromWithPio(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IMultiSwitch> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS28E04-100")), m_context(context), m_io(io),
   m_ioA(boost::make_shared<yApi::historization::CSwitch>("io_A")),
   m_ioB(boost::make_shared<yApi::historization::CSwitch>("io_B"))
{
   BOOST_ASSERT_MSG(m_identification->family() == k4kEepromWithPio, "Invalid family number");

   m_keywords.push_back(m_ioA);
   m_keywords.push_back(m_ioB);
}

C4kEepromWithPio::~C4kEepromWithPio()
{
}

boost::shared_ptr<const IIdentification> C4kEepromWithPio::ident() const
{
   return m_identification;
}

void C4kEepromWithPio::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioA))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioA);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioB))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioB);
}

void C4kEepromWithPio::historize()
{
   m_ioA->set(m_io->read(0));
   m_ioB->set(m_io->read(1));

   m_context->historizeData(ident()->deviceName(), m_keywords);
}

void C4kEepromWithPio::set(const std::string& keyword, const std::string& command)
{
   boost::shared_ptr<yApi::historization::CSwitch> kw;
   unsigned char unit;

   if (m_ioA->getKeyword() == keyword)
   {
      kw = m_ioA;
      unit = 0;
   }
   else if (m_ioB->getKeyword() == keyword)
   {
      kw = m_ioB;
      unit = 1;
   }
   else
   {
      YADOMS_LOG(error) << "Unknown keyword " << keyword;
      return;
   }

   if (kw->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
   {
      YADOMS_LOG(error) << "Try to drive the read-only keyword " << keyword;
      return;
   }
   kw->setCommand(command);
   m_io->write(unit, kw->get());
}

} // namespace device
