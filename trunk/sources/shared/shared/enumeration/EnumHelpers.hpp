#pragma once

#include "IExtendedEnum.h"
#include "shared/exception/Exception.hpp"
#include "shared/exception/OutOfRange.hpp"
/*

This class contains macros for defining extended macros which add the ability to use string and/or values for enum

//////////////////////////////////////////////
//MACRO usage
//////////////////////////////////////////////

   //////////////////////////////////////////
   //The header part
   //////////////////////////////////////////
   DECLARE_ENUM_HEADER(Command,
      ((Stop)(0))
      ((Open)(1))
      ((Close)(2))
   );

   or with SHARED
   
   DECLARE_ENUM_HEADER_SHARED(Command, YADOMS_SHARED_EXPORT,
   ((Stop)(0))
   ((Open)(1))
   ((Close)(2))
   );

   //////////////////////////////////////////
   //The implementation part
   //////////////////////////////////////////
   DECLARE_ENUM_IMPLEMENTATION(Command,
      (Stop)
      (Open)
      (Close)
   );   
   
   DECLARE_ENUM_IMPLEMENTATION_NESTED(CCurtain::, Command,
      (Stop)
      (Open)
      (Close)
   );




//////////////////////////////////////////////
//MACRO expansion
//////////////////////////////////////////////

   //////////////////////////////////////////
   //The header part 
   //////////////////////////////////////////
   class ECommand: public shared::enumeration::IExtendedEnum 
   {
      public: enum domain 
      {
         kStop = 0 , 
         kOpen = 1 , 
         kClose = 2 ,
      };
      ECommand();
      ECommand(domain value);
      ECommand(const std::string & valueAsString);
      ECommand(const int valueAsInt);
      virtual ~ECommand();
      operator int() const;
      operator std::string() const;
      ECommand & operator=(int const& obj);
      ECommand & operator=(std::string const& obj);
      ECommand & operator=(const ECommand & obj);
      ECommand const operator() () const;
      const std::string & getAsString() const;
      void setFromString(const std::string & val);
   private: 
      domain m_value;
      static const std::string StopString;
      static const std::string OpenString;
      static const std::string CloseString;
   };

   //////////////////////////////////////////
   //The header part with SHARED
   //////////////////////////////////////////
   //same as above; with export
   class __declspec(dllexport) ECommand: public shared::enumeration::IExtendedEnum.....





   //////////////////////////////////////////
   //The implementation part
   //////////////////////////////////////////
   const std::string ECommand::StopString = boost::algorithm::to_lower_copy( std::string("Stop") );
   const std::string ECommand::OpenString = boost::algorithm::to_lower_copy( std::string("Open") );
   const std::string ECommand::CloseString = boost::algorithm::to_lower_copy( std::string("Close") );

   ECommand::ECommand() 
      : m_value( kStop ) 
   {
   }
   
   ECommand::ECommand(domain value) 
      : m_value(value) 
   {
   }
   
   ECommand::ECommand(const std::string & valueAsString) 
   {
      setFromString(valueAsString);
   }
   ECommand::ECommand(const int valueAsInt) 
      : m_value((domain)valueAsInt) 
   {
   }
   
   ECommand::~ECommand() 
   {
   }
   
   ECommand::operator int() const 
   {
      return m_value;
   }
   ECommand::operator std::string() const 
   {
      return getAsString();
   }
   ECommand const ECommand::operator() () const 
   {
      return m_value;
   }
   ECommand & ECommand::operator=(int const& obj) 
   {
      m_value = (domain)obj;
      return *this;
   }
   ECommand & ECommand::operator=(const ECommand & obj) 
   {
      m_value = obj.m_value;
      return *this;
   }
   ECommand & ECommand::operator=(std::string const& obj) 
   {
      setFromString(obj);
      return *this;
   }
   const std::string & ECommand::getAsString() const 
   {
      switch (m_value) 
      {
      case kStop: 
         return StopString;
      case kOpen: 
         return OpenString;
      case kClose: 
         return CloseString;
      default: 
         throw shared::exception::COutOfRange("Invalid enum value");
      }
   }

   void ECommand::setFromString(const std::string & val)
   {
      if (boost::iequals(val, StopString)) 
         m_value = kStop; 
      else if (boost::iequals(val, OpenString)) 
         m_value = kOpen; 
      else if (boost::iequals(val, CloseString)) 
         m_value = kClose; 
      else   
         throw shared::exception::COutOfRange(val);
   }
  


  //////////////////////////////////////////
  //The implementation part for nested
  //////////////////////////////////////////
  //same as above, with container class specifier

  const std::string CCurtain::ECommand::StopString = boost::algorithm::to_lower_copy( std::string("Stop") );
  
  CCurtain::ECommand::ECommand()
  : m_value( kStop )
  {
  }

*/


//
/// \brief In the sequence of enum values, the first column is the name
//
#define ENUM_COLUMN_NAME   0

//
/// \brief In the sequence of enum values, the second column is the value
//
#define ENUM_COLUMN_VALUE  1

//
/// \brief Give the real enum name (appending a E beofre the enumName)
//
#define ENUM_CLASSNAME(_enumName) _enumName

//
/// \brief Give the name of a value (in implementation) : (Off) -> kOff
//
#define ENUM_EXTRACT_NAME_IMPL(_seq) BOOST_PP_CAT(k, _seq)
//
/// \brief Give the variable name of an enum value string  (in implementation): (Off) -> OffString
//
#define ENUM_EXTRACT_CONST_NAME_IMPL(_seq) BOOST_PP_CAT(_seq, String)

//
/// \brief Give the name of a value (in header) : ((Off)(0)) -> kOff
//
#define ENUM_EXTRACT_NAME(_seq) BOOST_PP_CAT(k, BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))
//
/// \brief Give the value (in header) : ((Off)(0)) -> 0
//
#define ENUM_EXTRACT_VALUE(_seq) BOOST_PP_SEQ_ELEM(ENUM_COLUMN_VALUE, _seq)
//
/// \brief Give the variable name of an enum value string (in header) : ((Off)(0)) -> OffString
//
#define ENUM_EXTRACT_CONST_NAME(_seq) ENUM_EXTRACT_CONST_NAME_IMPL(BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))


//
/// \brief Macro used to declare one enum value
//
#define DECLARE_DOMAIN_VALUE(r, _enumName, elem)   ENUM_EXTRACT_NAME(elem) = ENUM_EXTRACT_VALUE(elem),								

//
/// \brief Macro used to declare all the enum values
//
#define DECLARE_DOMAIN_VALUES(_seq)	BOOST_PP_SEQ_FOR_EACH(DECLARE_DOMAIN_VALUE, _, _seq)     

//
/// \brief Macro used to declare in header the string of one value
//
#define ENUM_DECLARE_STATIC_CONST_NAME(r, _enumName, elem)	static const std::string ENUM_EXTRACT_CONST_NAME(elem);

//
/// \brief Macro used to declare in header the string of each value
//
#define ENUM_DECLARE_STATIC_CONST_NAMES(_seq)   BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME, _, _seq)     



//
/// \brief Macro used to declare the Enum class header with possibility of export/import from/to a shared library
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _export       The export class specifier (can be __declspec(dllexport/dllimport) for MSVC, or extern "C" for unix systems
/// \param [in] _seq          The enuemration sequence
//
#define DECLARE_ENUM_HEADER_SHARED(_enumName, _export, _seq)								   \
   class _export ENUM_CLASSNAME(_enumName): public shared::enumeration::IExtendedEnum  \
	{                                                                                   \
	public:                                                                             \
		enum domain {																						   \
			DECLARE_DOMAIN_VALUES(_seq)																   \
		};																										   \
																												   \
		ENUM_CLASSNAME(_enumName)();																	   \
		ENUM_CLASSNAME(_enumName)(domain value);													   \
		ENUM_CLASSNAME(_enumName)(const std::string & valueAsString);	    				   \
		ENUM_CLASSNAME(_enumName)(const int valueAsInt);	    				               \
		virtual ~ENUM_CLASSNAME(_enumName)();														   \
		operator int() const;																			   \
		operator std::string() const;																	   \
		ENUM_CLASSNAME(_enumName) & operator=(int const& obj);								   \
		ENUM_CLASSNAME(_enumName) & operator=(std::string const& obj);						   \
		ENUM_CLASSNAME(_enumName) & operator=(const ENUM_CLASSNAME(_enumName) & obj);	   \
		ENUM_CLASSNAME(_enumName) const operator() () const;						            \
		const std::string & getAsString() const;													   \
		void setFromString(const std::string & val);												   \
	private:																									   \
		domain   m_value;																					   \
		ENUM_DECLARE_STATIC_CONST_NAMES(_seq);														   \
	};																											

//
/// \brief Macro used to declare the Enum class header 
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enuemration sequence ((On)(0)) ((Off)(1)) ((Dim)(2))
//
#define DECLARE_ENUM_HEADER(_enumName, _seq)	DECLARE_ENUM_HEADER_SHARED(_enumName, , _seq)			



//
/// \brief Macro used to declare the implementation of GetAsString method for one case
//
#define ENUM_DECLARE_GETASSTRING(r, _enumName, elem)	case ENUM_EXTRACT_NAME_IMPL(elem): return ENUM_EXTRACT_CONST_NAME_IMPL(elem);

//
/// \brief Macro used to declare the implementation of GetAsString method
//
#define ENUM_DECLARE_GETASSTRING_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_GETASSTRING, _, _seq)     

   
   
//
/// \brief Macro used to declare the implementation of SetFromString method for one case
//
#define ENUM_DECLARE_SETFROMSTRING(r, _enumName, elem)									      \
   if (boost::iequals(val, ENUM_EXTRACT_CONST_NAME_IMPL(elem)))                        \
      m_value = ENUM_EXTRACT_NAME_IMPL(elem);                                          \
   else                                                                                

   
//
/// \brief Macro used to declare the implementation of SetFromString method
//
#define ENUM_DECLARE_SETFROMSTRING_IMPL(_seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_SETFROMSTRING, _, _seq)     


//
/// \brief Macro used to declare the definition of one static enum name string
//   
#define ENUM_DECLARE_STATIC_CONST_NAME_IMPL(r, _fullEnumName, elem) const std::string _fullEnumName::ENUM_EXTRACT_CONST_NAME_IMPL(elem) = boost::algorithm::to_lower_copy( std::string(BOOST_PP_STRINGIZE(elem)) );


//
/// \brief Macro used to declare the definition of static enum name strings
//   
#define ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_fullClassifiedEnumName, _seq) BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME_IMPL, _fullClassifiedEnumName, _seq)     



//
/// \brief Macro used to declare the Enum class implementation with possibility of defining it as a nested class
/// \param [in] _fullClassifiedEnumName The enumeration name if nested : ex : CMyClass::EMyEnum
/// \param [in] __enumName       	The namespace to use without full qualified name : EMyEnum
/// \param [in] _seq          The enumeration sequence (Off)(On)(Dim)
//
#define DECLARE_ENUM_IMPLEMENTATION_NESTED(_fullClassifiedEnumName, _enumName, _seq)                                                                          \
   ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_fullClassifiedEnumName, _seq)                                                                      \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)() : m_value( BOOST_PP_CAT(k, BOOST_PP_SEQ_HEAD(_seq)) ) {}            \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(domain value) : m_value(value) {}                                     \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const std::string & valueAsString) { setFromString(valueAsString); }  \
   _fullClassifiedEnumName::ENUM_CLASSNAME(_enumName)(const int valueAsInt) : m_value((domain)valueAsInt) { }               \
   _fullClassifiedEnumName::~ENUM_CLASSNAME(_enumName)() {}                                                                 \
   _fullClassifiedEnumName::operator int() const { return m_value; }                                                        \
   _fullClassifiedEnumName::operator std::string() const { return getAsString(); }                                          \
   _fullClassifiedEnumName const _fullClassifiedEnumName::operator() () const { return m_value; }						      \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(int const& obj)                                           \
   {                                                                                                                          \
      m_value = (domain)obj;                                                                                                  \
      return *this;                                                                                                           \
   }                                                                                                                          \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(const ENUM_CLASSNAME(_enumName) & obj)                    \
   {                                                                                                                          \
      m_value = obj.m_value;                                                                                                  \
      return *this;                                                                                                           \
   }                                                                                                                          \
   _fullClassifiedEnumName & _fullClassifiedEnumName::operator=(std::string const& obj)                                   \
   {                                                                                                                          \
      setFromString(obj);                                                                                                     \
      return *this;                                                                                                           \
   }                                                                                                                          \
   const std::string & _fullClassifiedEnumName::getAsString() const                                                         \
	{                                                                                                                          \
		switch (m_value)                                                                                                        \
		{                                                                                                                       \
		ENUM_DECLARE_GETASSTRING_IMPL(_seq)                                                                                     \
		default: throw shared::exception::COutOfRange("Invalid enum value");                                                    \
		}                                                                                                                       \
	}                                                                                                                          \
	void _fullClassifiedEnumName::setFromString(const std::string & val)                                                     \
	{                                                                                                                          \
		ENUM_DECLARE_SETFROMSTRING_IMPL(_seq)                                                                                   \
      throw shared::exception::COutOfRange(val);                                                                              \
	}
   
//
/// \brief Macro used to declare the Enum class implementation
/// \param [in] _enumName     The enumeration name : Test : will give "enum ETest {..."
/// \param [in] _seq          The enuemration sequence (Off)(On)(Dim)
//   
#define DECLARE_ENUM_IMPLEMENTATION(_enumName, _seq)  DECLARE_ENUM_IMPLEMENTATION_NESTED(_enumName, _enumName, _seq)
