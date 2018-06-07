#include "stdafx.h"
#include "Type.h"

namespace notification { namespace change {

   DECLARE_ENUM_IMPLEMENTATION(EChangeType,
      ((Create))
      ((Update))
      ((Delete))
      ((Blacklist))
   )     
   
} // namespace change   
} // namespace notification
