#pragma once
#include "ContentType.h"
#include "IFormDataPartStringHandler.h"
#include "IFormDataPartFileHandler.h"
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

         /// @brief Create form data part handler for string
         /// @return Created form data part handler
         virtual boost::shared_ptr<IFormDataPartStringHandler> createFormDataPartStringHandler() = 0;

         /// @brief Create form data part handler for file
         /// @param path Folder where file will be created
         /// @return Created form data part handler
         virtual boost::shared_ptr<IFormDataPartFileHandler> createFormDataPartFileHandler(const boost::filesystem::path& path) = 0;

         /// @brief Read multipart parts using configurer handlers
         /// @param partsHandlers The part handlers map (pair of part-name / part-handler)
         virtual void readParts(const std::map<std::string, boost::shared_ptr<IFormDataPartHandler>>& partsHandlers) = 0;
      };
   } //namespace rest
} //namespace web 
