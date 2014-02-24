#pragma once

#include "EntityHelpers.hpp"

#define DELCARE_FIELD_CACQUISITION(t,n)   DELCARE_FIELD(CAcquisition,t,n)


namespace database { 
namespace entities {

   class CAcquisition
   {
   public:
      CAcquisition()
      {
      }

      virtual ~CAcquisition()
      {
      }

      DELCARE_FIELD_CACQUISITION(int, Id);
      DELCARE_FIELD_CACQUISITION(std::string, Source);
      DELCARE_FIELD_CACQUISITION(std::string, Keyword);
      DELCARE_FIELD_CACQUISITION(std::string, Value);
      DELCARE_FIELD_CACQUISITION(boost::posix_time::ptime, Date);
   };
} //namespace entities
} //namespace database

