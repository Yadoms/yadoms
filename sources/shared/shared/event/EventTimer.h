#pragma once
#include <shared/Export.h>
#include "ITimeEvent.h"

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	    A timer for events system
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CEventTimer : public ITimeEvent
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] eventId        Id of the event
      /// \param[in] periodic       true if the timer is periodic, false if timer is one-shot
      /// \param[in] period         Timer period. If provided, timer starts immediatley, else user must call start method
      //--------------------------------------------------------------
      CEventTimer(int eventId, bool periodic = false,
         const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CEventTimer();

      //--------------------------------------------------------------
      /// \brief	    Start the timer
      /// \param[in] period         Timer period. If not provided, use the period provided at construction
      //--------------------------------------------------------------
      void start(const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time);

      // ITimeEvent Implementation
      virtual boost::posix_time::ptime getNextStopPoint() const;
      virtual void reset();
      virtual bool canBeRemoved() const;
      virtual int getId() const;
      // [END] ITimeEvent Implementation

   private:
      //--------------------------------------------------------------
      /// \brief	    The event ID associated with the timer
      //--------------------------------------------------------------
      const int m_id;

      //--------------------------------------------------------------
      /// \brief	    Is the timer periodic (true), or one-shot
      //--------------------------------------------------------------
      const bool m_periodic;

      //--------------------------------------------------------------
      /// \brief	    The constant timer period (if provided at construction, boost::date_time::not_a_date_time else)
      //--------------------------------------------------------------
      const boost::posix_time::time_duration m_period;

      //--------------------------------------------------------------
      /// \brief	    The next stop point
      //--------------------------------------------------------------
      boost::posix_time::ptime m_nextStopPoint;
   };

} } // namespace shared::event
