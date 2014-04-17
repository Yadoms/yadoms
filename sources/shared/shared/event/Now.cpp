#include "stdafx.h"
#include "Now.h"

namespace shared { namespace event
{

const boost::posix_time::ptime now()
{
   return boost::posix_time::microsec_clock::universal_time();
}

} } // namespace shared::event
