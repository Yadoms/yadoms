#pragma once
#include "../../task/IInstance.h"

namespace notification { namespace taskProgression
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
      ///\param[in] taskProgression  The task progression data
      ///\note : onNotify is called in the caller thread-context. So, when implementing this method,
      // be careful on be atomic and do not throw exception. The best is to only post event to another thread.
      //-----------------------------------------------------
      virtual void onNotify(boost::shared_ptr<const task::IInstance> taskProgression) = 0;
   };

} } // namespace notification::taskProgression
	
	