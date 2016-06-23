#include "stdafx.h"
#include "DualAddressableSwitchPlus1kMemory.h"
#include "Identification.h"

namespace device
{
   CDualAddressableSwitchPlus1kMemory::CDualAddressableSwitchPlus1kMemory(EOneWireFamily family,
                                                                          const std::string& id,
                                                                          boost::shared_ptr<ioInterfaces::IDualAddressableSwitchPlus1kMemory> io)
      :m_identification(boost::make_shared<device::CIdentification>(family, id, "DS2406")),
       m_io(io),
       m_ioA(boost::make_shared<yApi::historization::CSwitch>("io_A")),
       m_keywords({ m_ioA })
   {
      BOOST_ASSERT_MSG(m_identification->family() == kDualAddressableSwitchPlus1kMemory, "Invalid family number");

      if (m_io->nbChannels() == 2)
      {
         m_ioB = boost::make_shared<yApi::historization::CSwitch>("io_B");
         m_keywords.push_back(m_ioB);
      }
   }

   CDualAddressableSwitchPlus1kMemory::~CDualAddressableSwitchPlus1kMemory()
   {
   }

   void CDualAddressableSwitchPlus1kMemory::read() const
   {
      m_ioA->set(m_io->read(0));

      if (!!m_ioB)
         m_ioB->set(m_io->read(1));
   }

   void CDualAddressableSwitchPlus1kMemory::write(const std::string& keyword, const std::string& command)
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
         if (!!m_ioB)
         {
            kw = m_ioB;
            unit = 1;
         }
         else
         {
            std::cerr << "Try to drive a non-active channel " << keyword << std::endl;
            return;
         }
      }
      else
      {
         std::cerr << "Unknown keyword " << keyword << std::endl;
         return;
      }

      if (kw->getAccessMode() != yApi::EKeywordAccessMode::kGetSetValue)
      {
         std::cerr << "Try to drive the read-only keyword " << keyword << std::endl;
         return;
      }
      kw->setCommand(command);
      m_io->write(unit, kw->get());
   }
} // namespace device
