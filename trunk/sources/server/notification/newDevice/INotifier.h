#pragma once
#include "../../database/sqlite/requesters/Device.h"
#include "IObserver.h"

namespace notification { namespace newDevice
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
      ///\param[in] newDevice  The new device event
      //-----------------------------------------------------
      virtual void post(boost::shared_ptr<const database::entities::CDevice> newDevice) = 0;
   };

} } // namespace notification::newDevice
	
	