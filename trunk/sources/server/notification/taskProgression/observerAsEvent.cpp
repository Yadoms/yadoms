#include "stdafx.h"
#include "observerAsEvent.h"

namespace notification { namespace taskProgression
{

CObserverAsEvent::CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId)
   :m_eventHandler(eventHandler), m_eventId(eventId)
{
}

CObserverAsEvent::~CObserverAsEvent()
{         
}

void CObserverAsEvent::onNotify(boost::shared_ptr<const task::IInstance> taskProgression)
{
   m_eventHandler.postEvent(m_eventId, taskProgression);
}

} } // namespace notification::taskProgression

