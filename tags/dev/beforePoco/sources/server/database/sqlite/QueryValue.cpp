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

   CQueryValue::CQueryValue(const std::string & value, bool secure /*= true*/) 
   { 
      if(secure)
         initialize("'" + value + "'");
      else
         initialize(value);
   }

   CQueryValue::CQueryValue(const boost::posix_time::ptime & anyValue, bool secure)
   {
      if(secure)
         initialize("'" + boost::posix_time::to_iso_string(anyValue) + "'");
      else
         initialize(boost::posix_time::to_iso_string(anyValue));
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
   
   CQueryValue& CQueryValue::operator=(const shared::CDataContainer & anyValue)
   {
      std::string valueAsString = anyValue.serialize();
      initialize(valueAsString);
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

