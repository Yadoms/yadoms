#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace startupOptions {
   //-----------------------------------------------------
   ///\brief   Database Engine
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(EDatabaseEngine, 
      ((Sqlite)(0))
      ((Postgresql)(1))
   )
} // namespace startupOptions
	
	