#include "stdafx.h"
#include "IO.h"
#include "staticInterrupts.h"
#include "pifacedigital.h"
#include "InitializationException.hpp"
#include <errno.h>

CIO::CIO(const std::string& keywordName,
         const int pin,
         const EPullResistance pullResistanceState, 
         const yApi::EKeywordAccessMode& accessMode,
         const shared::event::CEventHandler* interruptEventHandler)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_portUsed(baseAddress + pin),
   m_InterruptEventHandler(interruptEventHandler)
{
   if ((pin<0) || (pin>8))
      throw CInitializationException("pin out of range");

   // Configuring the access type
   //if ( accessMode == yApi::EKeywordAccessMode::kGetSet)
      //pinMode (baseAddress + pin, OUTPUT);

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
   {
      m_interruptReceiverThread = boost::thread(&CIO::interruptReceiverThreaded, this, pin, keywordName, ioManager);
      //if (wiringPiISR(baseAddress + pin, INT_EDGE_BOTH, interrupt[pin] ) == -1)
      //   throw CInitializationException( strerror (errno) );

      //pinMode (baseAddress + pin, INPUT);
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
            //pullUpDnControl (m_portUsed, PUD_OFF );
            //pifacedigital_write_reg(0xff, GPPUB, hw_addr);
            break;
         case kPullUp:
            //pullUpDnControl (m_portUsed, PUD_DOWN );
            //pifacedigital_write_reg(0xff, GPPUB, hw_addr);
            break;
         case kPullDown:
            //pullUpDnControl (m_portUsed, PUD_UP );
            break;
      }
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}

void CIO::interruptReceiverThreaded(const int portUsed, const std::string& keywordName, const boost::shared_ptr<CIOManager> ioManager) const
{
   try
   {
      while (true)
      {
         int value = digitalRead(portUsed);
         CIOState Event = { portUsed, keywordName, (bool)value };
         m_Event.postEvent<const CIOState>(kEvtIOStateReceived, Event);
      }
   }
   catch (boost::thread_interrupted&)
   {
   }
}