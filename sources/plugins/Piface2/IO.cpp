#include "stdafx.h"
#include "IO.h"

CIO::CIO(const std::string& keywordName, 
         const int pin, 
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode)),
   m_InterruptEventHandler(NULL),
   m_InterruptEventId(shared::event::kNoEvent),
   m_portUsed(pin),
   m_Writable(false)
{
   // TODO : implémenter ici la configuration d'un port
}

CIO::~CIO()
{
}

void CIO::write(bool state)
{
   std::cout << m_value->getKeyword() << "set to " << state << std::endl;
   m_value->set( state );
}

void CIO::subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler, int forId)
{
   m_InterruptEventHandler = &forEventHandler;
   m_InterruptEventId = forId;
}

void CIO::notifyEventHandler( void )
{
   // TODO : Read the value with wiringPi

   //set the statut
   m_value->set(true);

   if (m_InterruptEventHandler)
      m_InterruptEventHandler->postEvent<const IOState>(m_InterruptEventId, { m_portUsed, m_value });
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}