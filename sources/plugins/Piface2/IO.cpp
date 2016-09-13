#include "stdafx.h"
#include "IO.h"
#include "staticInterrupts.h"
#include "wiringPi.h"
#include "InitializationException.hpp"
#include <errno.h>

CIO::CIO(const std::string& keywordName, 
         const int pin,
         const EPullResistance pullResistanceState, 
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_portUsed(pin)
{
   // Configuring the access type
   if ( accessMode == yApi::EKeywordAccessMode::kGetSet)
      pinMode (pin, OUTPUT);

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
   {
      if (wiringPiISR( pin, INT_EDGE_BOTH, interrupt[pin] ) == -1)
         throw CInitializationException( strerror (errno) );

      //pinMode (pin, INPUT);
      ConfigurePullResistance ( pullResistanceState );
   }
}

CIO::~CIO()
{
}

void CIO::set(bool state, bool boardAccess)
{
   std::cout << m_value->getKeyword() << "set to " << state << std::endl;
   m_value->set( state );

   if (boardAccess) writeHardware (state);
}

void CIO::writeHardware(bool state)
{
   // Writing the value
   if (state)
      digitalWrite (m_portUsed, HIGH);
   else
      digitalWrite (m_portUsed, LOW);
}

void CIO::ConfigurePullResistance(const EPullResistance pullResistanceState)
{
      switch ( pullResistanceState )
      {
         case kDisable:
            pullUpDnControl (m_portUsed, PUD_OFF );
            break;
         case kPullUp:
            pullUpDnControl (m_portUsed, PUD_DOWN );
            break;
         case kPullDown:
            pullUpDnControl (m_portUsed, PUD_UP );
            break;
      }
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}
