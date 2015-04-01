#pragma once
#include "IObserver.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace acquisition
{
   //-----------------------------------------------------
   ///\brief An acquisition observer, used to wait for new acquisitions (and associated summaries)
   ///\note If only needed to wait for acquisition, CAcquisitionObserver is easier to use
   //-----------------------------------------------------
   class CObserverAsEvent : public IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] eventHandler Event handler used to post new acquisitions (see note for post data)
      ///\param[in] eventId      Event id posted with new acquisitions
      ///\param[in] keywordId The keywordId we want to be notified (use kAll to be notified on all keywords)
      ///\param[in] withSummaries true if summaries are needed
      ///\note data posted are :
      ///    - if only last acquisistion is needed (withSummaries=false), data posted are boost::shared_ptr<const database::entities::CAcquisition>
      ///    - if acquisistion with summaries is needed (withSummaries=true), data posted are
      ///      boost::tuple<boost::shared_ptr<const database::entities::CAcquisition>,    // Last acquisition
      ///         boost::shared_ptr<const database::entities::CAcquisitionSummary>,       // daily summary
      ///         boost::shared_ptr<const database::entities::CAcquisitionSummary> >      // hourly summary
      //-----------------------------------------------------
      CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId, int keywordId = kAll, bool withSummaries = false);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserverAsEvent();

   protected:
      // IObserver implementation
      virtual int keyword() const;
      virtual void onNotify(boost::shared_ptr<const database::entities::CAcquisition> acquisition,
         boost::shared_ptr<const database::entities::CAcquisitionSummary> dailySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>(),
         boost::shared_ptr<const database::entities::CAcquisitionSummary> hourlySummary = boost::shared_ptr<const database::entities::CAcquisitionSummary>());
      // [END] IObserver implementation

   private:
      //-----------------------------------------------------
      ///\brief               Event Handler
      //-----------------------------------------------------
      shared::event::CEventHandler& m_eventHandler;

      //-----------------------------------------------------
      ///\brief               Event ID
      //-----------------------------------------------------
      const int m_eventId;

      //-----------------------------------------------------
      ///\brief               Keyword ID (kAll if observe all keywords)
      //-----------------------------------------------------
      const int m_keywordId;

      //-----------------------------------------------------
      ///\brief               true if summaries are needed
      //-----------------------------------------------------
      bool m_withSummaries;
   };

} } // namespace notification::acquisition
	
	