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
      /// \brief Get the enum value as its string representation
      /// \return the enum value as string
      //--------------------------------------------------------------
      virtual const std::string & toString() const = 0;

      //--------------------------------------------------------------
      /// \brief Set the enum value from its string representation
      /// \param [in]   val   the string which will be parsed as the enum
      //--------------------------------------------------------------
      virtual void fromString(const std::string & val) = 0;

      //--------------------------------------------------------------
      /// \brief Virtual destructor
      //--------------------------------------------------------------
      virtual ~IExtendedEnum() 
      {
      }
   };

} //namespace enumeration
} //namespace shared