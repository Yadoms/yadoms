#pragma once

#include <shared/Export.h>

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	   Alias to get time reference for calculations in the class
   /// \return    The current time
   //--------------------------------------------------------------
   const YADOMS_SHARED_EXPORT boost::posix_time::ptime now();

} } // namespace shared::event
