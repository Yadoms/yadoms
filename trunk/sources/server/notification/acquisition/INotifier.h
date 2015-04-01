#pragma once
#include "../../database/sqlite/requesters/Acquisition.h"
#include "IObserver.h"

namespace notification { namespace acquisition
{
   //-----------------------------------------------------
   ///\brief The acquisition notifier interface
   //-----------------------------------------------------
   class INotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~INotifier() {}

      //-----------------------------------------------------
      ///\brief               Register an observer
      ///\param[in] observer  The observer to subscribe
      //-----------------------------------------------------
      virtual void subscribe(boost::shared_ptr<IObserver> observer) = 0;

      //-----------------------------------------------------
      ///\brief               Register an observer
      ///\param[in] observer  The observer to unsubscribe (do nothing if not found)
      //-----------------------------------------------------
      virtual void unsubscribe(boost::shared_ptr<IObserver> observer) = 0;

      //-----------------------------------------------------
      ///\brief               Post the new acquisition
      ///\param[in] acquisition  The new acquisition
      ///\param[in] dailySummary The new daily summary
      ///\param[in] hourlySummary The new hourly summary
      //-----------------------------------------------------
      virtual void post(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>()) = 0;
   };

} } // namespace notification::acquisition
	
	