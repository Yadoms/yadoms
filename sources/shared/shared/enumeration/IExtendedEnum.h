#pragma once

namespace shared { namespace enumeration {

   //--------------------------------------------------------------
   /// \brief Interface for extended enums (can be parsed/get as strings
   //--------------------------------------------------------------
   class IExtendedEnum
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