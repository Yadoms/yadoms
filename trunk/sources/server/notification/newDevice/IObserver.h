#pragma once
#include "../../database/entities/entities.h"

namespace notification { namespace newDevice
{
   //-----------------------------------------------------
   ///\brief The new device observer interface
   //-----------------------------------------------------
   class IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IObserver() {}

      //-----------------------------------------------------
      ///\brief               Notify callback
      ///\param[in] newDevice The new device
      ///\note : onNotify is called in the caller thread-context. So, when implementing this method,
      // be careful on be atomic and do not throw exception. The best is to only post event to another thread.
      //-----------------------------------------------------
      virtual void onNotify(boost::shared_ptr<const database::entities::CDevice> newDevice) = 0;
   };

} } // namespace notification::newDevice
	
	