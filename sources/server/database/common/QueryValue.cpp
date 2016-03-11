#include "stdafx.h"
#include "QueryValue.h"
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>

namespace database { 
namespace common {
   /*
   CQueryValue::CQueryValue() 
      :m_bIsDefined(false)
   { 
   }

   CQueryValue::~CQueryValue() 
   {
   }

   std::string CQueryValue::normalize(const std::string & value)
   {
      return boost::replace_all_copy(value, "'", "''");
   }

   CQueryValue::CQueryValue(const std::string & value, bool secure) 
   { 
      std::string normalizedValue = normalize(value);
      if (secure)
         normalizedValue = "'" + normalizedValue + "'";

      initialize(normalizedValue);
   }

   CQueryValue::CQueryValue(const boost::posix_time::ptime & anyValue, bool secure)
   {
      if(secure)
         initialize("'" + boost::posix_time::to_iso_string(anyValue) + "'");
      else
         initialize(boost::posix_time::to_iso_string(anyValue));
   }


   
   CQueryValue::CQueryValue(const Poco::DateTime & anyValue, bool secure)
   {
      std::string dateAsString = Poco::DateTimeFormatter::format(anyValue, "%Y%m%dT%H%M%S");

      if (secure)
         initialize("'" + dateAsString + "'");
      else
         initialize(dateAsString);

   }   
   
   CQueryValue::CQueryValue(const Poco::Timestamp & anyValue, bool secure)
   {
      std::string dateAsString = Poco::DateTimeFormatter::format(anyValue, "%Y%m%dT%H%M%S");

      if (secure)
         initialize("'" + dateAsString + "'");
      else
         initialize(dateAsString);
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
   */

} //namespace common
} //namespace database 

