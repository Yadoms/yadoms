#pragma once

#include <shared/enumeration/EnumHelpers.hpp>


namespace notification { namespace change {

   //-----------------------------------------------------
   ///\brief   The type of change notification
   ///<         - kCreate : This is a creation notification
   ///<         - kUpdate : This is an update notification
   ///<         - kDelete : This is a delete notification
   //-----------------------------------------------------
   DECLARE_ENUM_HEADER(EChangeType,
      ((Create)(0))
      ((Update)(1))
      ((Delete)(2))
      ((Blacklist)(3))
   )      
   
} // namespace change
} // namespace notification
