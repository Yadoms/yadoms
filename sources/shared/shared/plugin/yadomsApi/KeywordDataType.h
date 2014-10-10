#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief Keyword type
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EKeywordDataType, YADOMS_SHARED_EXPORT,
      ((NoData)(0)) 
      ((String)(1))
      ((Numeric)(2))
      ((Bool)(3))
      ((Json)(4))
   )

} } } // namespace shared::plugin::yadomsApi	
	
	