#include "stdafx.h"

#include "JsonDate.h"

namespace web { namespace rest { namespace json {

   std::string CJsonDate::toString(boost::posix_time::ptime p1)
   {
      return boost::posix_time::to_iso_string(p1);
      /*std::stringstream ss;
      boost::posix_time::time_facet facet("%Y%m%d%H%M%S");
      ss.imbue(std::locale(ss.getloc(), &facet));
      ss << p1;
      return ss.str();*/
   }

   boost::posix_time::ptime CJsonDate::fromString(const std::string & timeAsString)
   {
      return boost::posix_time::from_iso_string(timeAsString);
      /*
      std::stringstream ss;
      boost::posix_time::ptime p1;

      boost::posix_time::time_facet facet("%Y%m%d%H%M%S");
      ss.imbue(std::locale(ss.getloc(), &facet));
      ss.str(timeAsString);
      ss >> p1;
      return p1;*/
   }



} //namespace json
} //namespace rest
} //namespace web 

