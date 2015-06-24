#include "stdafx.h"
#include "UriSinglePatternValidator.h"
#include <boost/regex.hpp>

namespace tools {
   namespace web {


      bool CUriSinglePatternValidator::isValid(const std::string & uriPart)
      {
         //\_\@\.\!\*\#
         boost::regex validator("^[a-zA-Z0-9_.@#-]{1,}$");
         boost::match_results<std::string::const_iterator> results;
         return boost::regex_match(uriPart, results, validator);
      }


   } //namespace web
} //namespace tools
