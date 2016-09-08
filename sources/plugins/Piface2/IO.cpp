#include "stdafx.h"
#include "IO.h"

DECLARE_ENUM_IMPLEMENTATION(EIOType,
((Read))
((Write))
);

CIO::CIO(const std::string& keywordName, 
         const int pin, 
         EIOType configuration)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName)),
   m_InterruptEventHandler(NULL),
   m_InterruptEventId(shared::event::kNoEvent),
   m_portUsed(pin),
   m_Writable(false)
{
   // TODO : implémenter ici la configuration d'un port

   if (configuration == EIOType::kWrite) m_Writable = true; else m_Writable = false;
}

CIO::~CIO()
{
}

void CIO::writeValue(bool state)
{
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

bool CIO::isWritable(void)
{
   return m_Writable;
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}