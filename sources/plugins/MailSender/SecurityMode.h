#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

//-----------------------------------------------------
///\brief The security mode for sending emails
//-----------------------------------------------------
DECLARE_ENUM_HEADER(ESecurityMode,
   ((None)(0))
   ((SSL)(1))
   ((TLS)(2))
   ((Automatic)(3))
);