#include "stdafx.h"
#include "Provider.h"
#include "../exception/NullReference.hpp"

namespace shared { namespace currentTime
{
   boost::shared_ptr<ICurrentTime> Provider::CurrentTimeInstance;

   Provider::Provider(boost::shared_ptr<ICurrentTime> currentTimeInstance)
   {
      CurrentTimeInstance = currentTimeInstance;
   }

   Provider::~Provider()
   {
   }

   boost::posix_time::ptime Provider::now()
   {
      if (!CurrentTimeInstance)
         throw exception::CNullReference("Current time");

      return CurrentTimeInstance->now();
   }

} } // namespace shared::currentTime
