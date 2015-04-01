#include "stdafx.h"
#include "observer.h"

namespace notification { namespace configurationUpdate
{

CObserver::CObserver()
{
}

CObserver::~CObserver()
{         
}

void CObserver::onNotify(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration)
{
   m_eventHandler.postEvent(shared::event::kUserFirstId, newConfiguration);
}

boost::shared_ptr<const database::entities::CConfiguration> CObserver::wait(const boost::posix_time::time_duration& timeout)
{
   if (m_eventHandler.waitForEvents(timeout) == shared::event::kTimeout)
      return boost::shared_ptr<const database::entities::CConfiguration>();

   return m_eventHandler.getEventData<boost::shared_ptr<const database::entities::CConfiguration> >();
}

} } // namespace notification::configurationUpdate

