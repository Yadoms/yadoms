#pragma once
#include "IObserver.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace newDevice
{
   //-----------------------------------------------------
   ///\brief An new device observer, used to wait for new device notifications
   //-----------------------------------------------------
   class CObserverAsEvent : public IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] eventHandler Event handler used to post new  acquisitions
      ///\param[in] eventId      Event id posted with new acquisitions
      //-----------------------------------------------------
      CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserverAsEvent();

   protected:
      // IObserver implementation
      virtual void onNotify(boost::shared_ptr<const database::entities::CDevice> newDevice);
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
   };

} } // namespace notification::newDevice
	
	