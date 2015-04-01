#include "stdafx.h"
#include "notifier.h"

namespace notification { namespace acquisition
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
   m_observers[observer->keyword()].insert(observer);
}

void CNotifier::unsubscribe(boost::shared_ptr<IObserver> observer)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);
   std::map<int, UniqueKeywordObservers>::iterator it = m_observers.find(observer->keyword());
   if (it == m_observers.end())
      return;

   it->second.erase(observer);
}

void CNotifier::post(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary, boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);

   // Notify observers of the acquisition keyword
   notifyObserversFor(acquisition->KeywordId, acquisition, dailySummary, hourlySummary);

   // Notify also observers on all acquisitions
   notifyObserversFor(IObserver::kAll, acquisition, dailySummary, hourlySummary);
}

void CNotifier::notifyObserversFor(int keywordId, boost::shared_ptr<const database::entities::CAcquisition> acquisition,
   boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary, boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_observersMutex);

   // Find the observers list associated with the acquisition keywordId
   std::map<int, UniqueKeywordObservers>::iterator it = m_observers.find(keywordId);
   if (it == m_observers.end())
      return;

   // Notify all observers of the list
   UniqueKeywordObservers& keywordObserver = it->second;
   for (UniqueKeywordObservers::const_iterator it = keywordObserver.begin(); it != keywordObserver.end(); ++it)
      (*it)->onNotify(acquisition, dailySummary, hourlySummary);
}

} } // namespace notification::acquisition
