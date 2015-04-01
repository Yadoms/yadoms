#include "stdafx.h"
#include "notifier.h"

namespace notification { namespace configurationUpdate
{

CNotifier::CNotifier()
{
}

CNotifier::~CNotifier()
{         
}

void CNotifier::subscribe(boost::shared_ptr<IObserver> observer)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);
   m_observers.insert(observer);
}

void CNotifier::unsubscribe(boost::shared_ptr<IObserver> observer)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);
   m_observers.erase(observer);
}

void CNotifier::post(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration)
{
   // Notify observers
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);

   // Notify all observers of the list
   for (std::set<boost::shared_ptr<IObserver> >::const_iterator it = m_observers.begin(); it != m_observers.end(); ++it)
      (*it)->onNotify(newConfiguration);
}

} } // namespace notification::configurationUpdate
