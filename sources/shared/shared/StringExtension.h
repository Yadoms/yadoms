#pragma once
#include "exception/Exception.hpp"
#include <cstdarg>
#include <Poco/Types.h>
#include <ostream>

namespace shared
{
   //
   /// \brief Static class that provide converters from const char * to data type 
   //
   class CStringExtension
   {
   public:
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Formatting methods
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      static std::string format(const char* szFormat, va_list& argPtr)
      {
         char c;
         const auto nSize = vsnprintf(&c, 1, szFormat, argPtr);
         const auto str = static_cast<char*>(malloc(sizeof(char) * (nSize + 1)));
         vsnprintf(str, nSize + 1, szFormat, argPtr);
         std::string result(str);

         return result;
      }

      static std::string format(const char* szFormat, ...)
      {
         va_list args;
         va_start(args, szFormat);
         const std::string stringResult = format(szFormat, args);
         va_end(args);

         return stringResult;
      }

      static std::string removeEol(const std::string& line)
      {
         const auto len = line.size();

         if (len > 1 && line[len - 2] == '\r' && line[len - 1] == '\n')
            return line.substr(0, len - 2);
         if (len > 0 && (line[len - 1] == '\r' || line[len - 1] == '\n'))
            return line.substr(0, len - 1);

         return line;
      }

      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Parsing methods which use std::stringstream
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////

      //Essaie de parser la chaine dans le type attendu et le renvoie
      //si ko une exception est levee
      //il est possible de specifier une base en 2eme parametre facultatif
      template <class T>
      static T parse(const std::string& s, std::ios_base& (*f)(std::ios_base&))
      {
         T result;
         if (!tryParse(s, result, f))
            throw exception::CException("Unable to parse " + s);
         return result;
      }

      //Essaie de parser la chaine dans le type attendu et le renvoie
      //si ko une exception est levee
      //il est possible de specifier une base en 2eme parametre facultatif
      template <class T>
      static T parse(const char* s, std::ios_base& (*f)(std::ios_base&))
      {
         const std::string str(s);
         return parse<T>(str, f);
      }


      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Parsing methods
      // Special case for gcc, template specialization need to be declared outside the class ;-(
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////

      //
      /// \brief        parse a string into the templated type using boost::lexical_cast
      /// \param [in]   value : the string to parse
      /// \template     T : the type of the parse result 
      /// \return       the string parsed into the templated type
      //
      template <class T>
      static T parse(const char* value);


      //
      /// \brief               To string converter : used to convert data to string, locale-independently
      /// \param[in] value     Value to convert
      /// \return              Converted value, using the C locale
      //
      template <typename T>
      static std::string cultureInvariantToString(const T& value);


      /// @brief Trim string at beginning (in place)
      /// @param s string to trim
      static void leftTrim(std::string& s);

      /// @brief Trim string at end (in place)
      /// @param s string to trim
      static void rightTrim(std::string& s);

      /// @brief Trim string at beginning and end (in place)
      /// @param s string to trim
      static void trim(std::string& s);

      /// @brief Trim string at beginning and end (in place)
      /// @param input Input string containing keys to replace
      /// @param replacements Map containing replacement strings
      /// @param keyEncapsulationStartToken Token starting the key in input string
      /// @param keyEncapsulationendToken Token ending the key in input string
      /// @example :
      /// input = "Replace {{first}} and {{second}} in my string"
      /// replacement = { { "first": "value1" }, { "second": "value2" } }
      /// ==> returns "Replace value1 and value2 in my string"
      static std::string replaceValues(const std::string& input,
                                       const std::map<std::string, std::string>& replacements,
                                       const std::string& keyEncapsulationStartToken = "{{",
                                       const std::string& keyEncapsulationendToken = "}}");
   };

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Special case for gcc, template specialization need to be declared outside the class ;-(
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////

   template <class T>
   T CStringExtension::parse(const char* value)
   {
      return boost::lexical_cast<T>(value);
   }

   template <>
   inline double CStringExtension::parse(const char* value)
   {
      return atof(value);
   }

   template <>
   inline float CStringExtension::parse(const char* value)
   {
      return static_cast<float>(atof(value));
   }

   template <>
   inline int CStringExtension::parse(const char* value)
   {
      return atoi(value);
   }

   template <>
   inline long CStringExtension::parse(const char* value)
   {
      return atol(value);
   }

   template <>
   inline std::string CStringExtension::parse(const char* value)
   {
      return value;
   }

   template <>
   inline bool CStringExtension::parse(const char* value)
   {
      std::istringstream iss(value);
      auto result = false;
      iss >> std::boolalpha >> result;
      return result;
   }

   template <typename T>
   std::string CStringExtension::cultureInvariantToString(const T& value)
   {
      std::ostringstream ss;
      ss.imbue(std::locale::classic()); // Use the C locale 
      ss << value;
      return ss.str();
   }

   template <>
   inline std::string CStringExtension::cultureInvariantToString(const Poco::UInt8& value)
   {
      std::ostringstream ss;
      ss.imbue(std::locale::classic()); // Use the C locale 
      ss << static_cast<int>(value);
      return ss.str();
   }
} // namespace shared
