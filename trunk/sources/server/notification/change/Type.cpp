#pragma once

#include "Type.h"

namespace notification { namespace change {

   DECLARE_ENUM_IMPLEMENTATION(EChangeType,
      ((Create))
      ((Update))
      ((Delete))
   )     
   
} // namespace change   
} // namespace notification
