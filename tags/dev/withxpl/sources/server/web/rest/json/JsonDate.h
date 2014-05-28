#pragma once

#include "Json.h"

namespace web { namespace rest { namespace json {

   class CJsonDate
   {
   private:
      CJsonDate() {}

   public:
      static std::string toString(boost::posix_time::ptime);
      static boost::posix_time::ptime fromString(const std::string & timeAsString);
   };


} //namespace json
} //namespace rest
} //namespace web 

