#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief Keyword access mode
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EKeywordAccessMode, YADOMS_SHARED_EXPORT,
      ((NoAccess)(0))
      ((Get)(1))
      ((Set)(2))
      ((GetSet)(3))
   )
	
} } } // namespace shared::plugin::yadomsApi	
	
	