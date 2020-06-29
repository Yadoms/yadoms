#include "HttpResponseHelper.h"
#include <regex>
#include "shared/Log.h"

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         std::string CHttpResponseHelper::getHttpResponseHeaderField(const std::string& headerFieldName,
                                                                     const std::string& httpResponse)
         {
            const boost::regex pattern(headerFieldName + ": ([^\\n$\\\\\\r]+)", boost::regex::icase);
            boost::smatch match;

            if (!boost::regex_search(httpResponse, match, pattern))
            {
               return "";
            }
            return match[1].str();
         }

         bool CHttpResponseHelper::isValidHttpResponse(const std::string& httpResponse)
         {
            const boost::regex pattern("^[A-Za-z]{4}");
            boost::smatch match;

            if (!boost::regex_search(httpResponse, match, pattern) && match[1].str() != "HTTP")
            {
               return false;
            }
            return true;
         }

         bool CHttpResponseHelper::isValidHttpStatusCode(const std::string& httpResponse)
         {
            const boost::regex pattern("\\s*(\\b[0-9]{3}\\b)\\s+(.+)");
            boost::smatch match;

            if (!boost::regex_search(httpResponse, match, pattern) && match[1].str() != "200")
            {
               return false;
            }
            return true;
         }

         int CHttpResponseHelper::getHttpStatusCode(std::string& input)
         {
            std::smatch match;
            try
            {
               const std::regex pattern(std::string("[[:digit:]]{3}"));
               if (!std::regex_search(input, match, pattern))
               {
                  throw;
               }
            }
            catch (const std::exception& e)
            {
               YADOMS_LOG(warning) << "Unable to get Http status code " << e.what();
            }
            return std::stoi(match[0]);
         }
      }
   }
}
