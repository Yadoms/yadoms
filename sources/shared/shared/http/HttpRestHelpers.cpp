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

      void CHttpRestHelpers::sendGetRequest(const std::string& url,
                                            const boost::function<void(
                                               const std::map<std::string, std::string>& receivedHeaders,
                                               const std::string& data)>& responseHandlerFct,
                                            const std::map<std::string, std::string>& headerParameters,
                                            const std::map<std::string, std::string>& parameters)
      {
         createHttpRestRequest(CCurlppHttpRestRequest::EType::kGet, url)
            ->withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpRestHelpers::sendGetRequest(const std::string& url,
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

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonGetRequest(const std::string& url,
                                                                             const std::map<std::string, std::string>& headerParameters,
                                                                             const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kGet, url)
            ->withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }

      void CHttpRestHelpers::sendPostRequest(const std::string& url,
                                             const std::string& body,
                                             const boost::function<void(
                                                const std::map<std::string, std::string>& receivedHeaders,
                                                const std::string& data)>& responseHandlerFct,
                                             const std::map<std::string, std::string>& headerParameters,
                                             const std::map<std::string, std::string>& parameters)
      {
         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPost, url)
            ->withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpRestHelpers::sendPostRequest(
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

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonPostRequest(
         const std::string& url,
         const std::string& body,
         const std::map<std::string, std::string>& headerParameters,
         const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPost, url)
            ->withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }

      void CHttpRestHelpers::sendHeadRequest(const std::string& url,
                                             const boost::function<void(
                                                const std::map<std::string, std::string>& receivedHeaders)>&
                                             responseHandlerFct,
                                             const std::map<std::string, std::string>& headerParameters,
                                             const std::map<std::string, std::string>& parameters)
      {
         createHttpRestRequest(CCurlppHttpRestRequest::EType::kHead, url)
            ->withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send([&responseHandlerFct](const std::map<std::string, std::string>& receivedHeaders,
                                        const std::string&)
            {
               responseHandlerFct(receivedHeaders);
            });
      }

      std::map<std::string, std::string> CHttpRestHelpers::sendHeadRequest(const std::string& url,
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

      void CHttpRestHelpers::sendPutRequest(const std::string& url, const std::string& body,
                                            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                                       const std::string& data)>& responseHandlerFct,
                                            const std::map<std::string, std::string>& headerParameters,
                                            const std::map<std::string, std::string>& parameters)
      {
         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPut, url)
            ->withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send(responseHandlerFct);
      }

      std::string CHttpRestHelpers::sendPutRequest(const std::string& url, const std::string& body,
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

      boost::shared_ptr<CDataContainer> CHttpRestHelpers::sendJsonPutRequest(const std::string& url,
                                                                             const std::string& body,
                                                                             const std::map<std::string, std::string>& headerParameters,
                                                                             const std::map<std::string, std::string>& parameters)
      {
         boost::shared_ptr<CDataContainer> out;

         createHttpRestRequest(CCurlppHttpRestRequest::EType::kPut, url)
            ->withBody(body)
            .withHeaderParameters(headerParameters)
            .withParameters(parameters)
            .send([&out](boost::shared_ptr<CDataContainer> data)
            {
               out = std::move(data);
            });

         return out;
      }
   }
} // namespace shared::http
