#pragma once

namespace shared
{
   namespace event
   {
      //--------------------------------------------------------------
      /// \brief	    Interface for time events
      //--------------------------------------------------------------
      class ITimeEvent
      {
      public:
         //--------------------------------------------------------------
         /// \brief	    Destructor
         //--------------------------------------------------------------
         virtual ~ITimeEvent()
         {
         }

         //--------------------------------------------------------------
         /// \brief	    Get the next stop point
         /// \return     Next stop point of the time event, boost::date_time::not_a_date_time if time event is not active
         /// \note       Be careful of a limitation : a time-point is not DST (Daylight Saving Time) compatible : for example, 
         //              a timepoint set after a DST change (ie change to summer hour) will not take account of time offset.
         //--------------------------------------------------------------
         virtual boost::posix_time::ptime getNextStopPoint() const = 0;

         //--------------------------------------------------------------
         /// \brief	    Reset the time event (stop it if non-periodic)
         //--------------------------------------------------------------
         virtual void reset() = 0;

         //--------------------------------------------------------------
         /// \brief	    Check if time event can be removed from the event handler time events list
         ///             (for example if no more make sense because no more event is to come).
         /// \return     true if the time event can be removed from the event handler list
         /// \note       Normaly only called by event handler
         //--------------------------------------------------------------
         virtual bool canBeRemoved() const = 0;

         //--------------------------------------------------------------
         /// \brief	    Id getter
         /// \return     Event id
         //--------------------------------------------------------------
         virtual int getId() const = 0;
      };
   }
} // namespace shared::event


