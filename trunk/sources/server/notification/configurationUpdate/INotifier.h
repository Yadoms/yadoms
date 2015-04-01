#pragma once
#include "../../database/sqlite/requesters/Configuration.h"
#include "IObserver.h"

namespace notification { namespace configurationUpdate
{
   //-----------------------------------------------------
   ///\brief The configuration update notifier interface
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
      ///\brief               Post the configuration update event
      ///\param[in] newDevice  The configuration update event
      //-----------------------------------------------------
      virtual void post(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration) = 0;
   };

} } // namespace notification::configurationUpdate
	
	