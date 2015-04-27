#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace update
{
   //-----------------------------------------------------
   ///\brief   The update type
   ///\note    Used by IUpdateSource to provide the update type
   ///<         - kYadoms : about updating yadoms
   ///<         - kPlugin : about updating a plugin
   ///<         - kWidget : about updating a widget
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(EUpdateType,
      ((Yadoms)(0))
      ((Plugin)(1))
      ((Widget)(2))
   )
	
   
} // namespace update
