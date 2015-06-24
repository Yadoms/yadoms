#pragma once
#include <shared/enumeration/EnumHelpers.hpp>

namespace task { 

   //-----------------------------------------------------
   ///\brief    Task status
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(ETaskStatus,
      ((Started)(0))
      ((Success)(1))
      ((Fail)(2))
   )

} //namespace task
