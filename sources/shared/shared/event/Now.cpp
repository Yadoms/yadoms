#include "stdafx.h"
#include "Now.h"

namespace shared { namespace event
{

boost::posix_time::ptime now()
{
   return boost::posix_time::microsec_clock::local_time();
}

} } // namespace shared::event
