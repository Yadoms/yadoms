#include "stdafx.h"
#include "observerAsEvent.h"

namespace notification { namespace acquisition
{

CObserverAsEvent::CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId, int keywordId, bool withSummaries)
   :m_eventHandler(eventHandler), m_eventId(eventId), m_keywordId(keywordId), m_withSummaries(withSummaries)
{
}

CObserverAsEvent::~CObserverAsEvent()
{         
}

int CObserverAsEvent::keyword() const
{
   return m_keywordId;
}

void CObserverAsEvent::onNotify(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
{
   if (m_withSummaries)
      m_eventHandler.postEvent(m_eventId, boost::make_tuple(acquisition, dailySummary, hourlySummary));
   else
      m_eventHandler.postEvent(m_eventId, acquisition);
}

} } // namespace notification::acquisition

