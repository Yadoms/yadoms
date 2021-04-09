#include "stdafx.h"
#include "HttpMethods.h"
#include <shared/exception/HttpException.hpp>
#include <curlpp/cURLpp.hpp>
#include <regex>
#include "HttpMethod.h"

namespace shared
{
   namespace http
   {
      void CHttpMethods::sendGetRequest(const std::string& url,
                                        const boost::function<void(
                                           const std::map<std::string, std::string>& receivedHeaders,
                                           const std::string& data)>& responseHandlerFct,
                                        const std::map<std::string, std::string>& headerParameters,
                                        const std::map<std::string, std::string>& parameters)
      {
         CHttpMethod(CHttpMethod::ERequest::kGet, url)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpMethods::sendGetRequest(const std::string& url,
                                               const std::map<std::string, std::string>& headerParameters,
                                               const std::map<std::string, std::string>& parameters)
      {
         std::string out;
         sendGetRequest(url,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = data;
                        },
                        headerParameters,
                        parameters);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::sendJsonGetRequest(const std::string& url,
                                                                         const std::map<std::string, std::string>& headerParameters,
                                                                         const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;
         sendGetRequest(url,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = processJsonResponse(receivedHeaders,
                                                     data);
                        },
                        headerParameters,
                        parameters);

         return out;
      }

      void CHttpMethods::sendPostRequest(const std::string& url,
                                         const std::string& body,
                                         const boost::function<void(
                                            const std::map<std::string, std::string>& receivedHeaders,
                                            const std::string& data)>& responseHandlerFct,
                                         const std::map<std::string, std::string>& headerParameters,
                                         const std::map<std::string, std::string>& parameters)
      {
         CHttpMethod(CHttpMethod::ERequest::kPost, url)
            .withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpMethods::sendPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters)
      {
         std::string out;
         sendPostRequest(url,
                         body,
                         [&out](const std::map<std::string, std::string>& receivedHeaders,
                                const std::string& data)
                         {
                            out = data;
                         },
                         headerParameters,
                         parameters);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::sendJsonPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;
         sendPostRequest(url,
                         body,
                         [&out](const std::map<std::string, std::string>& receivedHeaders,
                                const std::string& data)
                         {
                            out = processJsonResponse(receivedHeaders,
                                                      data);
                         },
                         headerParameters,
                         parameters);

         return out;
      }

      void CHttpMethods::sendHeadRequest(const std::string& url,
                                         const boost::function<void(
                                            const std::map<std::string, std::string>& receivedHeaders)>&
                                         responseHandlerFct,
                                         const std::map<std::string, std::string>& headerParameters,
                                         const std::map<std::string, std::string>& parameters)
      {
         CHttpMethod(CHttpMethod::ERequest::kHead, url)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send([&responseHandlerFct](const std::map<std::string, std::string>& receivedHeaders,
                                        const std::string&)
            {
               responseHandlerFct(receivedHeaders);
            });
      }

      std::map<std::string, std::string> CHttpMethods::sendHeadRequest(const std::string& url,
                                                                       const std::map<std::string, std::string>& headerParameters,
                                                                       const std::map<std::string, std::string>& parameters)
      {
         std::map<std::string, std::string> out;
         sendHeadRequest(url,
                         [&out](const std::map<std::string, std::string>& receivedHeaders)
                         {
                            out = receivedHeaders;
                         },
                         headerParameters,
                         parameters);

         return out;
      }

      void CHttpMethods::sendPutRequest(const std::string& url, const std::string& body,
                                        const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                                   const std::string& data)>& responseHandlerFct,
                                        const std::map<std::string, std::string>& headerParameters,
                                        const std::map<std::string, std::string>& parameters)
      {
         CHttpMethod(CHttpMethod::ERequest::kPut, url)
            .withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpMethods::sendPutRequest(const std::string& url, const std::string& body,
                                               const std::map<std::string, std::string>& headerParameters,
                                               const std::map<std::string, std::string>& parameters)
      {
         std::string out;
         sendPutRequest(url,
                        body,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = data;
                        },
                        headerParameters,
                        parameters);

         return out;
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::sendJsonPutRequest(const std::string& url,
                                                                         const std::string& body,
                                                                         const std::map<std::string, std::string>& headerParameters,
                                                                         const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;
         sendPutRequest(url,
                        body,
                        [&out](const std::map<std::string, std::string>& receivedHeaders,
                               const std::string& data)
                        {
                           out = processJsonResponse(receivedHeaders,
                                                     data);
                        },
                        headerParameters,
                        parameters);

         return out;
      }

      std::string CHttpMethods::urlEncode(const std::string& url) //TODO utile ?
      {
         return cURLpp::escape(url);
      }

      std::string CHttpMethods::urlDecode(const std::string& url) //TODO utile ?
      {
         return cURLpp::unescape(url);
      }

      boost::shared_ptr<CDataContainer> CHttpMethods::processJsonResponse(
         const std::map<std::string, std::string>& receivedHeaders,
         const std::string& data)
      {
         try
         {
            const auto& contentType = receivedHeaders.at("content-type");
            if (contentType.find("application/json") == std::string::npos &&
               contentType.find("text/json") == std::string::npos)
               throw std::runtime_error(
                  std::string("Content type was not defined as JSON or text : ") + contentType);
         }
         catch (const std::exception& exception)
         {
            throw std::runtime_error(std::string("Fail to process HTTP JSON answer : ") + exception.what());
         }

         // Content-Length is not always fulfilled so we don't use hasContentLength and getContentLength
         return CDataContainer::make(data);
      }
   }
} // namespace shared::http
