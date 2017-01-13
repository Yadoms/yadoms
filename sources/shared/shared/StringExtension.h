#pragma once
#include <shared/Export.h>
#include "exception/Exception.hpp"
#include <stdarg.h>
#include <Poco/Types.h>

namespace shared
{
   //
   /// \brief Static class that provide converters from const char * to data type 
   //
   class YADOMS_SHARED_EXPORT CStringExtension
   {
   public:
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      // Formatting methods
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      static std::string format(const char *szFormat, va_list &arg_ptr )
	   {
		   char c;
	      auto nSize = vsnprintf(&c, 1, szFormat, arg_ptr);
	      auto str = static_cast<char *>(malloc(sizeof(char) * (nSize + 1)));
		   vsnprintf(str, nSize + 1, szFormat, arg_ptr);
		   std::string result(str);

		   return result;
	   }

	   static std::string format(const char * szFormat,...)
	   {
		   std::string stringResult;

		   va_list args;
		   va_start(args, szFormat);
		   stringResult = format(szFormat, args);
		   va_end (args);

		   return stringResult;
	   }

      static std::string removeEol(const std::string& line)
      {
         auto len = line.size();
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

      //Essaie de parser la chaine dans le type result et renvoie vrai si ok
	   //il est possible de specifier une base en 3eme parametre facultatif
	   template <class T>
	   static bool tryParse(const std::string & s, T& result, std::ios_base& (*f)(std::ios_base&) = std::dec)
	   {
		   std::istringstream iss(s);
		   return !(iss >> std::boolalpha >> f >> result).fail();
	   }

	   //Essaie de parser la chaine dans le type attendu et le renvoie
	   //si ko une exception est levee
	   //il est possible de specifier une base en 2eme parametre facultatif
	   template <class T>
	   static T parse(const std::string & s, std::ios_base& (*f)(std::ios_base&))
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
	   static T parse(const char * s, std::ios_base& (*f)(std::ios_base&))
	   {
		   std::string str(s);
		   return parse<T>(str);
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
      template<class T>
      static T parse(const char * value);
  

      //
      /// \brief               To string converter : used to convert data to string, locale-independently
      /// \param[in] value     Value to convert
      /// \return              Converted value, using the C locale
      //
      template<typename T>
      static std::string cultureInvariantToString(const T& value);
   };

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   // Special case for gcc, template specialization need to be declared outside the class ;-(
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////////////

   template<class T>
   T CStringExtension::parse(const char * value)
   {
      return boost::lexical_cast<T>(value);
   }

   template<>
   inline double CStringExtension::parse(const char * value)
   {
      return atof(value);
   }

   template<>
   inline float CStringExtension::parse(const char * value)
   {
      return static_cast<float>(atof(value));
   }

   template<>
   inline int CStringExtension::parse(const char * value)
   {
      return atoi(value);
   }

   template<>
   inline long CStringExtension::parse(const char * value)
   {
      return atol(value);
   }

   template<>
   inline std::string CStringExtension::parse(const char * value)
   {
      return value;
   }

   template<>
   inline bool CStringExtension::parse(const char * value)
   {
      std::istringstream iss(value);
      auto result = false;
      iss >> std::boolalpha >> result;      
      return result;
   }

   template<typename T>
   std::string CStringExtension::cultureInvariantToString(const T& value)
   {
      std::ostringstream ss;
      ss.imbue(std::locale::classic()); // Use the C locale 
      ss << value;
      return ss.str();
   }

   template<>
   inline std::string CStringExtension::cultureInvariantToString(const Poco::UInt8& value)
   {
      std::ostringstream ss;
      ss.imbue(std::locale::classic()); // Use the C locale 
      ss << static_cast<int>(value);
      return ss.str();
   }
} // namespace shared
