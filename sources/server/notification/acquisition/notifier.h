#pragma once
#include "INotifier.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace acquisition
{
   //-----------------------------------------------------
   ///\brief The acquisition notifier
   //-----------------------------------------------------
   class CNotifier : public INotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CNotifier();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CNotifier();

   protected:
      // INotifier implementation
      virtual void subscribe(boost::shared_ptr<IObserver> observer);
      virtual void unsubscribe(boost::shared_ptr<IObserver> observer);
      virtual void post(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>());
      // [END] INotifier implementation

      //-----------------------------------------------------
      ///\brief               Notify observers on a specific keyword
      ///\param[in] keywordId Keyword to notify observers
      ///\param[in] acquisition Acquisition to send to observers
      ///\param[in] dailySummary The new daily summary
      ///\param[in] hourlySummary The new hourly summary
      //-----------------------------------------------------
      void notifyObserversFor(int keywordId, boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary, boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary);

   private:
      //-----------------------------------------------------
      ///\brief               Subscribed observers
      //-----------------------------------------------------
      typedef std::set<boost::shared_ptr<IObserver> > UniqueKeywordObservers;
      std::map<int, UniqueKeywordObservers> m_observers;

      //-----------------------------------------------------
      ///\brief               Mutex for subscribed observers
      //-----------------------------------------------------
      mutable boost::recursive_mutex m_observersMutex;
   };

} } // namespace notification::acquisition
	
	