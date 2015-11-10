#include "stdafx.h"
#include "ReleaseType.h"

namespace shared { namespace versioning {

   DECLARE_ENUM_IMPLEMENTATION(EReleaseType,
      ((Stable))
      ((Testing))
      ((Beta))
   )

}} // namespace shared::versioning
	