#pragma once
#include "ITimeEvent.h"

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    A timer for events system
      /// \note       CEventTimer is not thread-safe, use it only on the thread calling waitForEvents of associated event handler
      //--------------------------------------------------------------
      class CEventTimer : public ITimeEvent
      {
      public:
         enum EPeriodicity
         {
            kPeriodic,
            kOneShot
         };


         //--------------------------------------------------------------
         /// \brief	    Constructor
         /// \param[in] eventId        Id of the event
         /// \param[in] periodicity       Periodic or one-shot timer
         /// \param[in] period         Timer period. If provided, timer starts immediately, else user must call start method
         //--------------------------------------------------------------
         explicit CEventTimer(int eventId,
                              EPeriodicity periodicity = kOneShot,
                              const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time);

         //--------------------------------------------------------------
         /// \brief	    Destructor
         //--------------------------------------------------------------
         virtual ~CEventTimer() = default;

         // Avoid copy
         CEventTimer(const CEventTimer&) = delete;
         const CEventTimer& operator=(const CEventTimer&) = delete;

         // Avoid move
         CEventTimer(const CEventTimer&&) = delete;
         const CEventTimer& operator=(const CEventTimer&&) = delete;

         //--------------------------------------------------------------
         /// \brief	    Start the timer
         /// \param[in] period         Timer period. If not provided, use the period provided at construction
         /// \throw std::invalid_argument if no period was found (not provided at construction, nor provided here)
         //--------------------------------------------------------------
         void start(const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time);

         //--------------------------------------------------------------
         /// \brief	    Stop the timer
         //--------------------------------------------------------------
         void stop();

         //--------------------------------------------------------------
         /// \brief	    Get running flag
         //--------------------------------------------------------------
         bool isRunning() const;

      protected:
         // ITimeEvent Implementation
         boost::posix_time::ptime getNextStopPoint() const override;
         void reset() override;
         bool canBeRemoved() const override;
         int getId() const override;
         // [END] ITimeEvent Implementation

         void doStart(const boost::posix_time::time_duration& period = boost::date_time::not_a_date_time);

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

         //--------------------------------------------------------------
         /// \brief	    Running flag
         //--------------------------------------------------------------
         bool m_isRunning;
      };
   }
} // namespace shared::event


