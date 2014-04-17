#pragma once
#include <shared/Export.h>
#include "ITimeEvent.h"

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	    A dated event for events system
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CEventTimePoint : public ITimeEvent
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Constructor
      /// \param[in] eventId        Id of the event  
      /// \param[in] dateTime       date/time when to raise the event, must be in the future
      //--------------------------------------------------------------
      CEventTimePoint(int eventId, const boost::posix_time::ptime& dateTime);

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~CEventTimePoint();
      
      // ITimeEvent Implementation
      virtual boost::posix_time::ptime getNextStopPoint() const;
      virtual void reset();
      virtual bool canBeDetached() const;
      virtual int getId() const;
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

} } // namespace shared::event
