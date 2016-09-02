#include "stdafx.h"
#include "ReleaseType.h"

namespace shared { namespace versioning {

   DECLARE_ENUM_IMPLEMENTATION(EReleaseType,
      ((Stable))
      ((ReleaseCandidate))
      ((Beta))
   )

}} // namespace shared::versioning
	