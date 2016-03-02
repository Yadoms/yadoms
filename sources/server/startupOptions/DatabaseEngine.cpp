#include "stdafx.h"
#include "DatabaseEngine.h"

namespace startupOptions {

   DECLARE_ENUM_IMPLEMENTATION(EDatabaseEngine,
      ((Sqlite)("sqlite"))
      ((Postgresql)("postgresql"))
   )

} // namespace startupOptions
	
	
	