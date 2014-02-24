#include "stdafx.h"
#include "QueryValue.h"


namespace database { 
namespace sqlite { 

   CQueryValue::CQueryValue() 
      :m_bIsDefined(false)
   { 
   }

   CQueryValue::~CQueryValue() 
   {
   }

   CQueryValue::CQueryValue(const std::string & value) 
   { 
      initialize("'" + value + "'");
   }

   CQueryValue::CQueryValue(const bool value) 
   { 
      initialize(std::string(value?"1":"0"));
   }

   CQueryValue& CQueryValue::operator=(const CQueryValue & anyValue)
   {
      initialize(anyValue.m_valueAsString);
      return *this;
   }

   const std::string & CQueryValue::str() const
   {
      return m_valueAsString;
   }



   const bool CQueryValue::isDefined() const 
   { 
      return m_bIsDefined;
   }

   void CQueryValue::initialize(const std::string & value)
   {
      m_valueAsString = value; 
      m_bIsDefined = true; 
   }


} //namespace sqlite
} //namespace database 

