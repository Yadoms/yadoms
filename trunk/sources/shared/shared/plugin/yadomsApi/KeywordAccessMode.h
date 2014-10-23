#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yadomsApi
{

   //-----------------------------------------------------
   ///\brief Keyword access mode
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER_SHARED(EKeywordAccessMode, YADOMS_SHARED_EXPORT,
      ((NoAccess))
      ((Get))
      ((GetSet))
   )
	
} } } // namespace shared::plugin::yadomsApi	
	
	