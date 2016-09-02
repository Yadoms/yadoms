#include "stdafx.h"
#include "SecurityMode.h"

DECLARE_ENUM_IMPLEMENTATION(ESecurityMode,
   ((None))
   ((SSL))
   ((TLS))
   ((Automatic))
);