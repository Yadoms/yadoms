#include "stdafx.h"
#include "IO.h"
#include "staticInterrupts.h"
#include "pifacedigital.h"
#include "InitializationException.hpp"

CIO::CIO(const std::string& keywordName,
         const int pin,
         const EPullResistance pullResistanceState, 
         const yApi::EKeywordAccessMode& accessMode,
         shared::event::CEventHandler& interruptEventHandler)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_portUsed(pin),
   m_InterruptEventHandler(interruptEventHandler)
{
   if ((pin<0) || (pin>8))
      throw CInitializationException("pin out of range");

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
   {
      m_interruptReceiverThread = boost::thread(&CIO::interruptReceiverThreaded, this, pin, keywordName);

      //TODO : A comprendre comment cela fonctionne
      m_interruptReceiverThread.interrupt();
      m_interruptReceiverThread.timed_join(boost::posix_time::seconds(20));

      if (!pifacedigital_enable_interrupts())
         throw CInitializationException("interrupt initialization error");

      ConfigurePullResistance ( pullResistanceState );
   }
}

CIO::~CIO()
{
}

void CIO::set(bool state, bool boardAccess)
{
   m_value->set( state );

   std::cout << m_value->getKeyword() << " set to " << state << std::endl;

   if (boardAccess) writeHardware (state);
}

void CIO::writeHardware(bool state)
{
   // Writing the value
   if (state)
      pifacedigital_digital_write(m_portUsed, 1);
   else
      pifacedigital_digital_write(m_portUsed, 0);
}

void CIO::ConfigurePullResistance(const EPullResistance pullResistanceState)
{
      switch ( pullResistanceState )
      {
         case kDisable:
            //pifacedigital_write_reg(0xff, GPPUB, hw_addr);
            break;
         case kPullUp:
            //pifacedigital_write_reg(0xff, GPPUB, hw_addr);
            break;
         case kPullDown: // Never used for Piface2
            break;
      }
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}

void CIO::interruptReceiverThreaded(const int portUsed, const std::string& keywordName) const
{
   try
   {
      while (true)
      {
         int value = pifacedigital_digital_read(portUsed);
         CIOState Event = { portUsed, keywordName, (bool)value };
         m_InterruptEventHandler->postEvent<const CIOState>(kEvtIOStateReceived, Event);
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}