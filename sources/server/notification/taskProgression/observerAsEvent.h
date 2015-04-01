#pragma once
#include "IObserver.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace taskProgression
{
   //-----------------------------------------------------
   ///\brief A task progression, used to wait for task progression notifications
   //-----------------------------------------------------
   class CObserverAsEvent : public IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] eventHandler Event handler used to post task progression
      ///\param[in] eventId      Event id posted with task progression
      //-----------------------------------------------------
      CObserverAsEvent(shared::event::CEventHandler& eventHandler, int eventId);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserverAsEvent();

   protected:
      // IObserver implementation
      virtual void onNotify(boost::shared_ptr<const task::IInstance> taskProgression);
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

} } // namespace notification::taskProgression
	
	