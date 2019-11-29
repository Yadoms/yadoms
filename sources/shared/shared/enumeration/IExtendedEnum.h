#pragma once

namespace shared
{
   namespace enumeration
   {
      //--------------------------------------------------------------
      /// \brief Interface for extended enums (can be parsed/get as strings)
      //--------------------------------------------------------------
      class IExtendedEnum
      {
      public:
         //--------------------------------------------------------------
         /// \brief Get the enumeration name
         /// \return the enumeration name
         //--------------------------------------------------------------
         virtual const std::string& getName() const = 0;

         //--------------------------------------------------------------
         /// \brief Get the enum value as its string representation
         /// \return the enum value as string
         //--------------------------------------------------------------
         virtual const std::string& toString() const = 0;

         //--------------------------------------------------------------
         /// \brief Set the enum value from its string representation
         /// \param [in]   val   the string which will be parsed as the enum
         //--------------------------------------------------------------
         virtual void fromString(const std::string& val) = 0;


         //--------------------------------------------------------------
         /// \brief Get the list of all values and strings
         /// \return	The list of all values (container is multimap<int, string> because enum could have duplicated values)
         //--------------------------------------------------------------
         virtual const std::multimap<int, std::string> getAllValuesAndStrings() const = 0;

         //--------------------------------------------------------------
         /// \brief Get the list of all values
         /// \return	The list of all values 
         //--------------------------------------------------------------
         virtual const std::vector<int> getAllValues() const = 0;

         //--------------------------------------------------------------
         /// \brief Get the list of all strings
         /// \return	The list of all strings 
         //--------------------------------------------------------------
         virtual const std::vector<std::string> getAllStrings() const = 0;

         //--------------------------------------------------------------
         /// \brief Virtual destructor
         //--------------------------------------------------------------
         virtual ~IExtendedEnum()
         {
         }
      };


      //--------------------------------------------------------------
      /// \brief Overload the << operator of std::ostream
      ///        The aim is to use a specific behavior when using IExtendedEnum with std::ostream
      ///         Without this overload, "std::ostream << IExtendedEnum" add the int value of std::ostream
      ///         With it, the string value is used
      //--------------------------------------------------------------
      std::ostream& operator <<(std::ostream& stream, const IExtendedEnum& enumValue);
   } //namespace enumeration
} //namespace shared


