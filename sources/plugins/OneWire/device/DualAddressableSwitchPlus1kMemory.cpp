#include "stdafx.h"
#include "DualAddressableSwitchPlus1kMemory.h"
#include "Identification.h"
#include <shared/Log.h>

namespace device {
   
CDualAddressableSwitchPlus1kMemory::CDualAddressableSwitchPlus1kMemory(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::IDualAddressableSwitchPlus1kMemory> io)
   :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2406")), m_context(context), m_io(io)
{
   BOOST_ASSERT_MSG(m_identification->family() == kDualAddressableSwitchPlus1kMemory, "Invalid family number");

   m_ioA = boost::make_shared<yApi::historization::CSwitch>("io_A");
   m_ioB = boost::make_shared<yApi::historization::CSwitch>("io_B");
}

CDualAddressableSwitchPlus1kMemory::~CDualAddressableSwitchPlus1kMemory()
{
}

boost::shared_ptr<const IIdentification> CDualAddressableSwitchPlus1kMemory::ident() const
{
   return m_identification;
}

void CDualAddressableSwitchPlus1kMemory::declare()
{
   m_identification->declare(m_context);

   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioA))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioA);
   if (!m_context->keywordExists(m_identification->deviceName(), *m_ioB))
      m_context->declareKeyword(m_identification->deviceName(), *m_ioB);
}

void CDualAddressableSwitchPlus1kMemory::historize()
{
   std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > keywords;

   m_ioA->set(m_io->read(0));
   keywords.push_back(m_ioA);

   if (m_io->nbChannels() == 2)
   {
      m_ioB->set(m_io->read(1));
      keywords.push_back(m_ioB);
   }

   m_context->historizeData(ident()->deviceName(), keywords);
}

void CDualAddressableSwitchPlus1kMemory::set(const std::string& keyword, const std::string& command)
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
      if (m_io->nbChannels() == 2)
      {
         kw = m_ioB;
         unit = 1;
      }
      else
      {
         YADOMS_LOG(error) << "Try to drive a non-active channel " << keyword;
         return;
      }
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
