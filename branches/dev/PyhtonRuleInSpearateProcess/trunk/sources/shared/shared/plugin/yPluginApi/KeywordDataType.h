#pragma once
#include <shared/Export.h>
#include <shared/enumeration/EnumHelpers.hpp>

namespace shared { namespace plugin { namespace yPluginApi
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
      ((Enum))
   )

} } } // namespace shared::plugin::yPluginApi	
	
	