#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace automation { namespace condition
{
   //-----------------------------------------------------
   ///\brief               Available comparison operators
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(EComparators,
      ((Equal))
      ((Different))
      ((Lower))
      ((LowerOrEqual))
      ((Greater))
      ((GreaterOrEqual))
   )

} } // namespace automation::condition	
	
	