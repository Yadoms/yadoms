#include "stdafx.h"
#include "IO.h"
#include <pifacedigital.h>
#include "InitializationException.hpp"
#include <mcp23s17.h>
#include <shared/Log.h>

CIO::CIO(const std::string& keywordName,
         const int pin,
         const EPullResistance pullResistanceState, 
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_portUsed(pin)
{
   if ((pin<0) || (pin>8))
      throw CInitializationException("pin out of range");

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
      ConfigurePullResistance ( pullResistanceState );
}

CIO::~CIO()
{}

void CIO::set(bool state)
{
   m_value->set( state );

   // Writing the value only if it's a getSet
   if ((m_value->getAccessMode() == yApi::EKeywordAccessMode::kGetSet))
   {
      if (state)
         pifacedigital_digital_write(m_portUsed, 1);
      else
         pifacedigital_digital_write(m_portUsed, 0);
   }
   
   YADOMS_LOG(trace) << m_value->getKeyword() << " set to " << state;
}

bool CIO::get(void)
{
   bool ret = pifacedigital_digital_read(m_portUsed);
   m_value->set(ret);

   YADOMS_LOG(information) << "read " << m_value->getKeyword() << " at " << ret;

   return ret;
}

void CIO::ConfigurePullResistance(const EPullResistance pullResistanceState)
{
   if ((m_value->getAccessMode() == yApi::EKeywordAccessMode::kGet))
   {	
      switch ( pullResistanceState )
      {
         case kDisable:
			pifacedigital_write_bit(0x00, m_portUsed, GPPUB, 0 );
			YADOMS_LOG(information) << "pull-up for " << m_value->getKeyword() << " disabled.";
            break;
         case kPullUp:
			pifacedigital_write_bit(0x01, m_portUsed, GPPUB, 0 );
			YADOMS_LOG(information) << "pull-up for " << m_value->getKeyword() << " enabled.";
            break;
         case kPullDown: // Never used for Piface2
            break;
         default:        // Never used also !
            break;
      }
   }
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}
