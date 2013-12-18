#pragma once
#include "tools/Exceptions/Exception.hpp"
#include <stdarg.h> 

//
/// \brief Static class that provide converters from const char * to data type 
//
struct CStringExtension
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
		int nSize = vsnprintf(&c, 1, szFormat, arg_ptr);
		char *str = (char *)malloc(sizeof(char) * (nSize + 1));
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
	static bool tryParse(const std::string & s, T& result, std::ios_base& (*f)(std::ios_base&))
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
			throw CException("Unable to parse " + s);
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
   static inline T parse(const char * value);
  


};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Special case for gcc, template specialization need to be declared outside the class ;-(
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
inline T CStringExtension::parse(const char * value)
{
   return boost::lexical_cast<T>(value);
}

//
/// \brief        parse a string into double (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string parsed into double
//   
template<>
inline double CStringExtension::parse(const char * value)
{
   return atof(value);
}

//
/// \brief        parse a string into float (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string parsed into float
//   
template<>
inline float CStringExtension::parse(const char * value)
{
   return (float)atof(value);
}

//
/// \brief        parse a string into integer (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string parsed into integer
//   
template<>
inline int CStringExtension::parse(const char * value)
{
   return atoi(value);
}

//
/// \brief        parse a string into long (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string parsed into long
//   
template<>
inline long CStringExtension::parse(const char * value)
{
   return atol(value);
}

//
/// \brief        parse a string into string (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string
//   
template<>
inline std::string CStringExtension::parse(const char * value)
{
   return value;
}

//
/// \brief        parse a string into bool (template specialisation)
/// \param [in]   value : the string to parse
/// \return       the string parsed into bool
//   
template<>
inline bool CStringExtension::parse(const char * value)
{
   std::istringstream iss(value);
   bool result;
   iss >> std::boolalpha >> result;      
   return result;
}   


