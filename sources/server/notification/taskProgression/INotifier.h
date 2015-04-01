#pragma once
#include "../../task/Instance.h"
#include "IObserver.h"

namespace notification { namespace taskProgression
{
   //-----------------------------------------------------
   ///\brief The new device notifier interface
   //-----------------------------------------------------
   class INotifier
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~INotifier() {}

      //-----------------------------------------------------
      ///\brief               Register an observer
      ///\param[in] observer  The observer to subscribe
      //-----------------------------------------------------
      virtual void subscribe(boost::shared_ptr<IObserver> observer) = 0;

      //-----------------------------------------------------
      ///\brief               Register an observer
      ///\param[in] observer  The observer to unsubscribe (do nothing if not found)
      //-----------------------------------------------------
      virtual void unsubscribe(boost::shared_ptr<IObserver> observer) = 0;

      //-----------------------------------------------------
      ///\brief               Post the new device event
      ///\param[in] taskProgression  The task progression data
      //-----------------------------------------------------
      virtual void post(boost::shared_ptr<const task::IInstance> taskProgression) = 0;
   };

} } // namespace notification::taskProgression
	
	