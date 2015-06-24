#pragma once

#include <shared/Export.h>

namespace shared { namespace event
{
   //--------------------------------------------------------------
   /// \brief	   Alias to get local time reference for calculations in the class
   /// \return    The current time
   //--------------------------------------------------------------
   boost::posix_time::ptime YADOMS_SHARED_EXPORT now();

} } // namespace shared::event
