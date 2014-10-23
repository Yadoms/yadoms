#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief Keyword type
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EKeywordDataType, YADOMS_SHARED_EXPORT,
      ((NoData)) 
      ((String))
      ((Numeric))
      ((Bool))
      ((Json))
   )

} } } // namespace shared::plugin::yadomsApi	
	
	