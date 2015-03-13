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

   std::string CQueryValue::replace(const std::string& s, const std::string& from, const std::string& to)
   {
      std::string out = s;
      for(size_t pos = 0; (pos = out.find(from, pos)) != std::string::npos; pos += to.size())
         out.replace(pos, from.size(), to);
      return out;
   }

   std::string CQueryValue::normalize(const std::string & value)
   {
      return replace(value, std::string("\'"), std::string("\'\'"));
   }

   CQueryValue::CQueryValue(const std::string & value, bool secure /*= true*/) 
   { 
      if(secure)
         initialize("'" + normalize(value) + "'");
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

