#pragma once
#include "ITimeEvent.h"

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    A dated event for events system
      /// \note       CEventTimePoint is not thread-safe, use it only on the thread calling waitForEvents of associated event handler
      //--------------------------------------------------------------
      class CEventTimePoint : public ITimeEvent
      {
      public:
         //--------------------------------------------------------------
         /// \brief	    Constructor
         /// \param[in] eventId        Id of the event  
         /// \param[in] dateTime       date/time when to raise the event, must be in the future. If not provided,
         ///                           caller has to call set() method.
         //--------------------------------------------------------------
         explicit CEventTimePoint(int eventId,
                                  const boost::posix_time::ptime& dateTime = boost::date_time::not_a_date_time);

         //--------------------------------------------------------------
         /// \brief	    Destructor
         //--------------------------------------------------------------
         virtual ~CEventTimePoint() = default;

         // Avoid copy
         CEventTimePoint(const CEventTimePoint&) = delete;
         const CEventTimePoint& operator=(const CEventTimePoint&) = delete;

         // Avoid move
         CEventTimePoint(const CEventTimePoint&&) = delete;
         const CEventTimePoint& operator=(const CEventTimePoint&&) = delete;

         //--------------------------------------------------------------
         /// \brief	    Set the time point
         /// \throw std::invalid_argument if provided dateTime value is not valid or not in the future
         /// \param[in] dateTime       date/time when to raise the event, must be in the future
         //--------------------------------------------------------------
         void set(const boost::posix_time::ptime& dateTime);

         //--------------------------------------------------------------
         /// \brief	    Cancel the time point
         //--------------------------------------------------------------
         void cancel();

      protected:
         // ITimeEvent Implementation
         boost::posix_time::ptime getNextStopPoint() const override;
         void reset() override;
         bool canBeRemoved() const override;
         int getId() const override;
         // [END] ITimeEvent Implementation

      private:
         //--------------------------------------------------------------
         /// \brief	    The event ID associated with the timer
         //--------------------------------------------------------------
         const int m_id;

         //--------------------------------------------------------------
         /// \brief	    The next stop point
         //--------------------------------------------------------------
         boost::posix_time::ptime m_dateTime;
      };
   }
} // namespace shared::event
