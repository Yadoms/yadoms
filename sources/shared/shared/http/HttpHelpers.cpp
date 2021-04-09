#include "stdafx.h"
#include "HttpHelpers.h"
#include <curlpp/cURLpp.hpp>

namespace shared
{
   namespace http
   {
      std::string CHttpHelpers::urlEncode(const std::string& url)
      {
         return cURLpp::escape(url);
      }

      std::string CHttpHelpers::urlDecode(const std::string& url)
      {
         return cURLpp::unescape(url);
      }
   }
} // namespace shared::http
