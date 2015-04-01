#pragma once
#include "IObserver.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace acquisition
{
   //-----------------------------------------------------
   ///\brief A simple acquisition observer, used to wait for (and only for) new acquisitions (no summaries)
   ///\note If needed to wait for acquisition in parallel of others events, use CAcquisitionObserverAsEvent
   //-----------------------------------------------------
   class CObserver : public IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] keywordId The keywordId we want to be notified (use kAll to be notified on all keywords)
      //-----------------------------------------------------
      CObserver(int keywordId = kAll);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserver();

      //-----------------------------------------------------
      ///\brief               Wait for simple acquisition (without summaries)
      ///\param[in] timeout  Set an optional timeout delay. Can use these special values :
      ///             - pos_infin : no timeout
      ///             - min_date_time : no wait, function returns immediately
      ///             is set to min_date_time special value.
      ///\return the new acquisition, null pointer if timeout
      //-----------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CAcquisition> wait(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

   protected:
      // IObserver implementation
      virtual int keyword() const;
      virtual void onNotify(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>());
      // [END] IObserver implementation

   private:
      //-----------------------------------------------------
      ///\brief               Keyword ID (kAll if observe all keywords)
      //-----------------------------------------------------
      const int m_keywordId;

      //-----------------------------------------------------
      ///\brief               Event Handler
      //-----------------------------------------------------
      shared::event::CEventHandler m_eventHandler;
   };

} } // namespace notification::acquisition
	
	