#include "stdafx.h"
#include "observer.h"

namespace notification { namespace acquisition
{

CObserver::CObserver(int keywordId)
   :m_keywordId(keywordId)
{
}

CObserver::~CObserver()
{         
}

int CObserver::keyword() const
{
   return m_keywordId;
}

void CObserver::onNotify(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
{
   m_eventHandler.postEvent(shared::event::kUserFirstId, acquisition);
}

boost::shared_ptr<const database::entities::CAcquisition> CObserver::wait(const boost::posix_time::time_duration& timeout)
{
   if (m_eventHandler.waitForEvents(timeout) == shared::event::kTimeout)
      return boost::shared_ptr<const database::entities::CAcquisition>();

   return m_eventHandler.getEventData<boost::shared_ptr<const database::entities::CAcquisition> >();
}

} } // namespace notification::acquisition

