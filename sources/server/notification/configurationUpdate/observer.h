#pragma once
#include "IObserver.h"
#include <shared/event/EventHandler.hpp>

namespace notification { namespace configurationUpdate
{
   //-----------------------------------------------------
   ///\brief An configuration update observer, used to wait for configuration update notifications
   //-----------------------------------------------------
   class CObserver : public IObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      //-----------------------------------------------------
      CObserver();

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CObserver();

      //-----------------------------------------------------
      ///\brief               Wait for configuration update
      ///\param[in] timeout  Set an optional timeout delay. Can use these special values :
      ///             - pos_infin : no timeout
      ///             - min_date_time : no wait, function returns immediately
      ///             is set to min_date_time special value.
      ///\return the new configuration, null pointer if timeout
      //-----------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CConfiguration> wait(const boost::posix_time::time_duration& timeout = boost::date_time::pos_infin);

   protected:
      // IObserver implementation
      virtual void onNotify(boost::shared_ptr<const database::entities::CConfiguration> newConfiguration);
      // [END] IObserver implementation

   private:
      //-----------------------------------------------------
      ///\brief               Event Handler
      //-----------------------------------------------------
      shared::event::CEventHandler m_eventHandler;
   };

} } // namespace notification::configurationUpdate
	
	