#include "stdafx.h"
#include "Callback.h"


namespace communication { namespace command {

   CCallback::CCallback()
   {
   }

   CCallback::~CCallback()
   {
   }

   void CCallback::sendResult(CResult & result)
   {
      postEvent(kResult, result);
   }

   CResult CCallback::waitForResult(const boost::posix_time::time_duration& timeout /*= boost::date_time::pos_infin*/)
   {
      if(waitForEvents(timeout) == kResult)
      {
         return getEventData<CResult>();
      }
      return CResult::CreateError("No answer.");
   }

} //namespace command
} //namespace communication
