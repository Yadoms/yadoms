// This code come from Paul Schlyter, thanks to him
// See http://stjarnhimlen.se/comp/sunriset.c

#include "stdafx.h"
#include "Location.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>


namespace automation { namespace script
{

CLocation::CLocation()
{
}

CLocation::~CLocation()
{         
}

double CLocation::latitude() const
{
   return 45.05;//TODO
}

double CLocation::longitude() const
{
   return 5.05;//TODO
}

double CLocation::altitude() const
{
   return 179;//TODO
}

} } // namespace automation::script

