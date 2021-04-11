#include "stdafx.h"
#include "HttpRestHelpers.h"

#include <utility>
#include "curlppHttpRestRequest.h"

namespace shared
{
   namespace http
   {
      boost::shared_ptr<IHttpRestRequest> CHttpRestHelpers::createHttpRestRequest(IHttpRestRequest::EType requestType,
                                                                                  const std::string& url)
      {
         return boost::make_shared<CCurlppHttpRestRequest>(requestType, url);
      }

      std::string CHttpRestHelpers::sendGetRequest(const std::string& url,
                                                   const std::map<std::string, std::string>& parameters)
      {
         std::string out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kGet, url)
            ->withParameters(parameters)
            .send([&out](const std::map<std::string, std::string>& receivedHeaders,
                         const std::string& data)
            {
               out = data;
            });

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonGetRequest(const std::string& url,
                                                                             const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kGet, url)
            ->withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }

      std::string CHttpRestHelpers::sendPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& parameters)
      {
         std::string out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPost, url)
            ->withBody(body)
            .withParameters(parameters)
            .send([&out](const std::map<std::string, std::string>& receivedHeaders,
                         const std::string& data)
            {
               out = data;
            });

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPost, url)
            ->withBody(body)
            .withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }

      std::map<std::string, std::string> CHttpRestHelpers::sendHeadRequest(const std::string& url,
                                                                           const std::map<std::string, std::string>& parameters)
      {
         std::map<std::string, std::string> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kHead, url)
            ->withParameters(parameters)
            .send([&out](const std::map<std::string, std::string>& receivedHeaders,
                         const std::string&)
            {
               out = receivedHeaders;
            });

         return out;
      }

      std::string CHttpRestHelpers::sendPutRequest(const std::string& url,
                                                   const std::string& body,
                                                   const std::map<std::string, std::string>& parameters)
      {
         std::string out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPut, url)
            ->withBody(body)
            .withParameters(parameters)
            .send([&out](const std::map<std::string, std::string>& receivedHeaders,
                         const std::string& data)
            {
               out = data;
            });

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonPutRequest(const std::string& url,
                                                                             const std::string& body,
                                                                             const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPut, url)
            ->withBody(body)
            .withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }
   }
} // namespace shared::http
