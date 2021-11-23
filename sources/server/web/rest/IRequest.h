#pragma once
#include "ContentType.h"
#include "shared/http/HttpRestVerb.h"

namespace web
{
   namespace rest
   {
      class IRequest
      {
      public:
         virtual ~IRequest() = default;

         virtual shared::http::ERestVerb method() const = 0;

         virtual bool pathVariableExists(const std::string& key) const = 0;
         virtual std::string pathVariable(const std::string& key) const = 0;
         virtual std::string pathVariable(const std::string& key,
                                          const std::string& defaultValue) const = 0;
         virtual std::unique_ptr<std::set<std::string>> pathVariableAsList(const std::string& key,
                                                                           char separator = '|') const = 0;

         virtual bool queryParamExists(const std::string& key) const = 0;
         virtual std::string queryParam(const std::string& key) const = 0;
         virtual std::string queryParam(const std::string& key,
                                        const std::string& defaultValue) const = 0;
         virtual std::unique_ptr<std::set<std::string>> queryParamAsList(const std::string& key,
                                                                         char separator = '|') const = 0;
         virtual boost::shared_ptr<const std::map<std::string, std::string>> queryParams() const = 0;

         virtual EContentType contentType() const = 0;
         virtual std::string body() const = 0;

         /// @brief Check if content type is accepted for answer
         /// @param contentType content type to test
         /// @return quality factor (see https://developer.mozilla.org/fr/docs/Web/HTTP/Headers/Accept)
         virtual float acceptContentType(EContentType contentType) const = 0;
      };
   } //namespace rest
} //namespace web 
