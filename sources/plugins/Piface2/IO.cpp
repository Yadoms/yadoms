#include "stdafx.h"
#include "IO.h"
#include "wiringPi.h"

CIO::CIO(const std::string& keywordName, 
         const int pin, 
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_InterruptEventHandler(NULL),
   m_InterruptEventId(shared::event::kNoEvent),
   m_portUsed(pin)
{

   // Configuring the access type
   if ( accessMode == yApi::EKeywordAccessMode::kGetSet)
      pinMode (pin, OUTPUT);

   if ( accessMode == yApi::EKeywordAccessMode::kGet)
      pinMode (pin, INPUT);
}

CIO::~CIO()
{
}

void CIO::write(bool state)
{
   std::cout << m_value->getKeyword() << "set to " << state << std::endl;
   m_value->set( state );

   // Writing the value
   if (state)
      digitalWrite (m_portUsed, HIGH);
   else
      digitalWrite (m_portUsed, LOW);
}

void CIO::subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler, int forId)
{
   m_InterruptEventHandler = &forEventHandler;
   m_InterruptEventId = forId;
}

void CIO::notifyEventHandler( void )
{
   //set the statut
   m_value->set( digitalRead ( m_portUsed ) );

   if (m_InterruptEventHandler)
      m_InterruptEventHandler->postEvent<const IOState>(m_InterruptEventId, { m_portUsed, m_value });
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}
