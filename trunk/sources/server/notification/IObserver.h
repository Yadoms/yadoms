#pragma once

#include "INotification.h"

namespace notification { 
   
   //-----------------------------
   ///\brief Interface for observer
   //-----------------------------
   class IObserver
   {
   public:
      //-----------------------------
      ///\brief Destructor
      //-----------------------------
      virtual ~IObserver() {}

      //-----------------------------
      ///\brief Observe the notification
      ///\param [in] notification   The notification to observe
      //-----------------------------
      virtual void observe(const boost::shared_ptr< INotification > notification) = 0;
   };

} // namespace notification
