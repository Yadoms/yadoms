#include "stdafx.h"
#include "IO.h"
#include "staticInterrupts.h"
//#include "wiringPi.h"
#include "pifacedigital.h"
#include "InitializationException.hpp"
#include <errno.h>

CIO::CIO(const std::string& keywordName,
         const int baseAddress,
         const int pin,
         const EPullResistance pullResistanceState, 
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_portUsed(baseAddress + pin)
{
   if ((pin<0) || (pin>8))
      throw CInitializationException("pin out of range");

   // Configuring the access type
   if ( accessMode == yApi::EKeywordAccessMode::kGetSet)
      //pinMode (baseAddress + pin, OUTPUT);

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
   {
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
      //digitalWrite (m_portUsed, HIGH);
      pifacedigital_digital_write(m_portUsed, 1);
   else
      //digitalWrite (m_portUsed, LOW);
      pifacedigital_digital_write(m_portUsed, 1);
}

void CIO::ConfigurePullResistance(const EPullResistance pullResistanceState)
{
      switch ( pullResistanceState )
      {
         case kDisable:
            //pullUpDnControl (m_portUsed, PUD_OFF );
            pifacedigital_write_reg(0xff, GPPUB, hw_addr);
            break;
         case kPullUp:
            //pullUpDnControl (m_portUsed, PUD_DOWN );
            pifacedigital_write_reg(0xff, GPPUB, hw_addr);
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