#pragma once
#include "../../database/entities/Entities.h"

namespace notification { namespace configurationUpdate
{
   //-----------------------------------------------------
   ///\brief The configuraiton update observer interface
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
      ///\param[in] newConfiguration  The new configuration
      ///\note : onNotify is called in the caller thread-context. So, when implementing this method,
      // be careful on be atomic and do not throw exception. The best is to only post event to another thread.
      //-----------------------------------------------------
      virtual void onNotify(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration) = 0;
   };

} } // namespace notification::configurationUpdate
	
	
