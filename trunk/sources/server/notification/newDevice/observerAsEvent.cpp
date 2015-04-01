#include "stdafx.h"
#include "observerAsEvent.h"

namespace notification { namespace newDevice
{

CObserverAsEvent::CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId)
   :m_eventHandler(eventHandler), m_eventId(eventId)
{
}

CObserverAsEvent::~CObserverAsEvent()
{         
}

void CObserverAsEvent::onNotify(boost::shared_ptr<const database::entities::CDevice> newDevice)
{
   m_eventHandler.postEvent(m_eventId, newDevice);
}

} } // namespace notification::newDevice

