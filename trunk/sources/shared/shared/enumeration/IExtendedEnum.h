#pragma once
#include <shared/Export.h>

namespace shared { namespace enumeration {

   //--------------------------------------------------------------
   /// \brief Interface for extended enums (can be parsed/get as strings
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT IExtendedEnum
   {
   public:
      //--------------------------------------------------------------
      /// \brief Virtual destructor
      //--------------------------------------------------------------
      virtual ~IExtendedEnum() 
      {
      }
   };

} //namespace enumeration
} //namespace shared