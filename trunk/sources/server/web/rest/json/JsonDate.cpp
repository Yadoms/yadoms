#include "stdafx.h"

#include "JsonDate.h"

namespace web { namespace rest { namespace json {

   std::string CJsonDate::toString(boost::posix_time::ptime p1)
   {
      return boost::posix_time::to_iso_string(p1);
   }

   boost::posix_time::ptime CJsonDate::fromString(const std::string & timeAsString)
   {
      return boost::posix_time::from_iso_string(timeAsString);
   }



} //namespace json
} //namespace rest
} //namespace web 

