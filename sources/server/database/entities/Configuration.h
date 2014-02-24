#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CCONFIGURATION(t,n)   DELCARE_FIELD(CConfiguration,t,n)


namespace database { 
namespace entities {

   class CConfiguration
   {
   public:
      enum ESecurityAccess
      {
         kNone = 0, 
         kAdmin = 1, 
         kUser = 2  
      };

   public:
      CConfiguration()
      {
      }

      virtual ~CConfiguration()
      {
      }

   public:
      DELCARE_FIELD_CCONFIGURATION(std::string, Section);
      DELCARE_FIELD_CCONFIGURATION(std::string, Name);
      DELCARE_FIELD_CCONFIGURATION(std::string, Value);
      DELCARE_FIELD_CCONFIGURATION(std::string, DefaultValue);
      DELCARE_FIELD_CCONFIGURATION(std::string, Description);
      DELCARE_FIELD_CCONFIGURATION(ESecurityAccess, SecurityAccess);
      DELCARE_FIELD_CCONFIGURATION(boost::posix_time::ptime, LastModificationDate);
   };
 
} //namespace entities
} //namespace database

