#include "stdafx.h"
#include "TaskStatus.h"

namespace task { 

   DECLARE_ENUM_IMPLEMENTATION(ETaskStatus,
      ((Started))
      ((Success))
      ((Fail))
   )

} //namespace task
