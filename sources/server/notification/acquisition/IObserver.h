#pragma once
#include "../../database/entities/Entities.h"

namespace notification { namespace acquisition
{
   //-----------------------------------------------------
   ///\brief The acquisition observer interface
   //-----------------------------------------------------
   class IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IObserver() {}

      //-----------------------------------------------------
      ///\brief               Get associated keyword ID
      ///\return              The keyword ID (kAll if observe all keywords)
      //-----------------------------------------------------
      enum { kAll = 0 };
      virtual int keyword() const = 0;

      //-----------------------------------------------------
      ///\brief               Notify callback
      ///\param[in] acquisition  The new acquisition
      ///\param[in] dailySummary The new daily summary
      ///\param[in] hourlySummary The new hourly summary
      ///\note : onNotify is called in the caller thread-context. So, when implementing this method,
      // be careful on be atomic and do not throw exception. The best is to only post event to another thread.
      //-----------------------------------------------------
      virtual void onNotify(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>()) = 0;
   };

} } // namespace notification::acquisition
	
	
