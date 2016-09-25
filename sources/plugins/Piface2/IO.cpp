#include "stdafx.h"
#include "IO.h"
#include "pifacedigital.h"
#include "InitializationException.hpp"
#include <mcp23s17.h>

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

   // Writing the value
   if (state)
      pifacedigital_digital_write(m_portUsed, 1);
   else
      pifacedigital_digital_write(m_portUsed, 0);

   std::cout << m_value->getKeyword() << " set to " << state << std::endl;
}

bool CIO::get(void)
{
   bool ret = pifacedigital_digital_read(m_portUsed);
   m_value->set(ret);

   return ret;
}

void CIO::ConfigurePullResistance(const EPullResistance pullResistanceState)
{
      switch ( pullResistanceState )
      {
         case kDisable:
            pifacedigital_write_reg(0x00, GPPUB, 0);
            break;
         case kPullUp:
            pifacedigital_write_reg(0xff, GPPUB, 0);
            break;
         case kPullDown: // Never used for Piface2
            break;
         default:        // Never used also !
            break;
      }
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}