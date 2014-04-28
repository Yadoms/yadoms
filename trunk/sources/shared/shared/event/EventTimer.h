#pragma once
#include <shared/Export.h>
#include "ITimeEvent.h"

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	    A timer for events system
   /// \note       CEventTimer is not thread-safe, use it only on the thread calling waitForEvents of associated event handler
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CEventTimer : public ITimeEvent
   {
   public:
      enum EPeriodicity
      {
         kPeriodic,
         kOneShot
      };

   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] eventId        Id of the event
      /// \param[in] periodic       Periodic or one-shot timer
      /// \param[in] period         Timer period. If provided, timer starts immediatley, else user must call start method
      //--------------------------------------------------------------
      CEventTimer(int eventId, EPeriodicity periodicity = kOneShot,
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

      //--------------------------------------------------------------
      /// \brief	    Stop the timer
      //--------------------------------------------------------------
      void stop();

   protected:
      friend class CEventHandler;
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
      /// \brief	    Timer periodicity
      //--------------------------------------------------------------
      const EPeriodicity m_periodicity;

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
