#pragma once
#include <shared/Export.h>

namespace shared { namespace event
{

   //--------------------------------------------------------------
   /// \brief	    Interface for time events
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT ITimeEvent
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~ITimeEvent() {}

      //--------------------------------------------------------------
      /// \brief	    Get the next stop point
      /// \return     Next stop point of the time event, boost::date_time::not_a_date_time if time event is not active
      //--------------------------------------------------------------
      virtual boost::posix_time::ptime getNextStopPoint() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Reset the time event (stop it if non-periodic)
      //--------------------------------------------------------------
      virtual void reset() = 0;

      //--------------------------------------------------------------
      /// \brief	    Check if time event can be detached (if no more make sense because
      ///             no more event is to come).
      /// \return     true if the time event no more make sense and can be detached from event handler
      //--------------------------------------------------------------
      virtual bool canBeDetached() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Id getter
      /// \return     Event id
      //--------------------------------------------------------------
      virtual int getId() const = 0;
   };

} } // namespace shared::event
