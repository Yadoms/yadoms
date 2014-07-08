#pragma once
   

//
/// \brief Macro used to declare the Enum class name
//

#define ENUM_COLUMN_NAME   0
#define ENUM_COLUMN_VALUE  1

#define ENUM_CLASSNAME(_enumName) E##_enumName


#define ENUM_EXTRACT_NAME_IMPL(_seq) BOOST_PP_CAT(k, _seq)
#define ENUM_EXTRACT_CONST_NAME_IMPL(_seq) BOOST_PP_CAT(_seq, String)

#define ENUM_EXTRACT_NAME(_seq) BOOST_PP_CAT(k, BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))
#define ENUM_EXTRACT_VALUE(_seq) BOOST_PP_SEQ_ELEM(ENUM_COLUMN_VALUE, _seq)
#define ENUM_EXTRACT_CONST_NAME(_seq) ENUM_EXTRACT_CONST_NAME_IMPL(BOOST_PP_SEQ_ELEM(ENUM_COLUMN_NAME, _seq))




#define DECLARE_DOMAIN_VALUE(r, _enumName, elem)   ENUM_EXTRACT_NAME(elem) = ENUM_EXTRACT_VALUE(elem),								
#define DECLARE_DOMAIN_VALUES(_seq)	BOOST_PP_SEQ_FOR_EACH(DECLARE_DOMAIN_VALUE, _, _seq)     



#define ENUM_DECLARE_STATIC_CONST_NAME(r, _enumName, elem)									\
	static const std::string ENUM_EXTRACT_CONST_NAME(elem);

#define ENUM_DECLARE_STATIC_CONST_NAMES(_seq)													\
	BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME, _, _seq)     






//
/// \brief  Declare a table
/// \param  name  the table name
/// \param  _seq  the sequence of columns
//
#define DECLARE_ENUM_HEADER(_enumName, _seq)													   \
class ENUM_CLASSNAME(_enumName) : public shared::enumeration::IExtendedEnum         \
	{                                                                                \
	public:                                                                          \
		enum domain {																						\
			DECLARE_DOMAIN_VALUES(_seq)																\
		};																										\
																												\
		ENUM_CLASSNAME(_enumName)();																	\
		ENUM_CLASSNAME(_enumName)(domain value);													\
		ENUM_CLASSNAME(_enumName)(const std::string & valueAsString);	    				\
		ENUM_CLASSNAME(_enumName)(const int valueAsInt);	    				            \
		virtual ~ENUM_CLASSNAME(_enumName)();														\
		operator int() const;																			\
		operator std::string() const;																	\
		ENUM_CLASSNAME(_enumName) & operator=(int const& obj);								\
		ENUM_CLASSNAME(_enumName) & operator=(std::string const& obj);						\
		ENUM_CLASSNAME(_enumName) & operator=(const ENUM_CLASSNAME(_enumName) & obj);	\
		ENUM_CLASSNAME(_enumName) const& operator() () const;						         \
		const std::string & getAsString() const;													\
		void setFromString(const std::string & val);												\
	private:																									\
		domain   m_value;																					\
		ENUM_DECLARE_STATIC_CONST_NAMES(_seq);														\
	};																											\


#define ENUM_DECLARE_GETASSTRING(r, _enumName, elem)									      \
   case ENUM_EXTRACT_NAME_IMPL(elem): return ENUM_EXTRACT_CONST_NAME_IMPL(elem);


#define ENUM_DECLARE_GETASSTRING_IMPL(_seq)													   \
	BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_GETASSTRING, _, _seq)     

   
   
   
#define ENUM_DECLARE_SETFROMSTRING(r, _enumName, elem)									      \
   if (boost::iequals(val, ENUM_EXTRACT_CONST_NAME_IMPL(elem)))                        \
      m_value = ENUM_EXTRACT_NAME_IMPL(elem);                                          \
   else                                                                                

   
#define ENUM_DECLARE_SETFROMSTRING_IMPL(_seq)													   \
	BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_SETFROMSTRING, _, _seq)     



#define ENUM_DECLARE_STATIC_CONST_NAME_IMPL(r, _enumName, elem)								\
   const std::string ENUM_CLASSNAME(_enumName)::ENUM_EXTRACT_CONST_NAME_IMPL(elem) = BOOST_PP_STRINGIZE(elem);

   
#define ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_enumName, _seq)													   \
	BOOST_PP_SEQ_FOR_EACH(ENUM_DECLARE_STATIC_CONST_NAME_IMPL, _enumName, _seq)     


#define DECLARE_ENUM_IMPLEMENTATION(_enumName, _seq)                                                                          \
   ENUM_DECLARE_STATIC_CONST_NAMES_IMPL(_enumName, _seq)                                                                      \
   ENUM_CLASSNAME(_enumName)::ENUM_CLASSNAME(_enumName)() : m_value( BOOST_PP_CAT(k, BOOST_PP_SEQ_HEAD(_seq)) ) {}            \
   ENUM_CLASSNAME(_enumName)::ENUM_CLASSNAME(_enumName)(domain value) : m_value(value) {}                                     \
   ENUM_CLASSNAME(_enumName)::ENUM_CLASSNAME(_enumName)(const std::string & valueAsString) { setFromString(valueAsString); }  \
   ENUM_CLASSNAME(_enumName)::ENUM_CLASSNAME(_enumName)(const int valueAsInt) : m_value((domain)valueAsInt) { }               \
   ENUM_CLASSNAME(_enumName)::~ENUM_CLASSNAME(_enumName)() {}                                                                 \
   ENUM_CLASSNAME(_enumName)::operator int() const { return m_value; }                                                        \
   ENUM_CLASSNAME(_enumName)::operator std::string() const { return getAsString(); }                                          \
   ENUM_CLASSNAME(_enumName) const& ENUM_CLASSNAME(_enumName)::operator() () const { return m_value; }						      \
   ENUM_CLASSNAME(_enumName) & ENUM_CLASSNAME(_enumName)::operator=(int const& obj)                                           \
   {                                                                                                                          \
      m_value = (domain)obj;                                                                                                  \
      return *this;                                                                                                           \
   }                                                                                                                          \
   ENUM_CLASSNAME(_enumName) & ENUM_CLASSNAME(_enumName)::operator=(const ENUM_CLASSNAME(_enumName) & obj)                    \
   {                                                                                                                          \
      m_value = obj.m_value;                                                                                                  \
      return *this;                                                                                                           \
   }                                                                                                                          \
   ENUM_CLASSNAME(_enumName) & ENUM_CLASSNAME(_enumName)::operator=(std::string const& obj)                                   \
   {                                                                                                                          \
      setFromString(obj);                                                                                                     \
      return *this;                                                                                                           \
   }                                                                                                                          \
   const std::string & ENUM_CLASSNAME(_enumName)::getAsString() const                                                         \
	{                                                                                                                          \
		switch (m_value)                                                                                                        \
		{                                                                                                                       \
		ENUM_DECLARE_GETASSTRING_IMPL(_seq)                                                                                     \
		default: throw shared::exception::COutOfRange("Invalid enum value");                                                    \
		}                                                                                                                       \
	}                                                                                                                          \
	void ENUM_CLASSNAME(_enumName)::setFromString(const std::string & val)                                                     \
	{                                                                                                                          \
		ENUM_DECLARE_SETFROMSTRING_IMPL(_seq)                                                                                   \
      throw shared::exception::COutOfRange(val);                                                                              \
	}
   
   

/*
class ESecurityAccess
{
public:
   enum domain
   {
      kNone = 0, 
      kAdmin = 1, 
      kUser = 2  
   };
   
   ESecurityAccess() {}
   ESecurityAccess(domain value)
      :m_value(value)
   {
   }
   
   //-----------------------------
   ///\brief Implicit cast operator 
   ///\return it returns the enum value as INT
   //-----------------------------
   operator int() const
   {
      return m_value;
   }
   
   //-----------------------------
   ///\brief Implicit cast operator 
   ///\return it returns the enum value as INT
   //-----------------------------
   operator std::string() const
   {
		return getAsString();
   }
   
   ESecurityAccess & operator=(int const& obj)
   {
      m_value = (domain)obj;
      return *this;
   }   
   
   ESecurityAccess & operator=(std::string const& obj)
   {
      setFromString(obj);
      return *this;
   }   
   
   
   
	//-----------------------------
	///\brief  Get the value as a string
	///\return the enum value as std::string
	//-----------------------------   
	const std::string & getAsString() const
	{
		
		switch (m_value)
		{
		case kNone: return kNoneString;
		case kAdmin: return kAdminString;
		case kUser: return kUserString;
		default: throw std::exception("Invalid enum value");
		}
	}

	void setFromString(const std::string & val)
	{
		if (boost::iequals(val, kNoneString))
			m_value = kNone;
		else if (boost::iequals(val, kAdminString)) 
			m_value = kAdmin;
		else if (boost::iequals(val, kUserString)) 
			m_value = kUser;
      else
			throw std::exception("Invalid enum value");
	}
private:
   domain   m_value;

	static const std::string kNoneString;
	static const std::string kAdminString;
	static const std::string kUserString;
};
*/