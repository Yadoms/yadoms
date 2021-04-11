#pragma once
#include <map>
#include <functional>
#include <string>

#include <curlpp/Easy.hpp>

#include "shared/DataContainer.h"
#include "IHttpRestRequest.h"


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot HTTP(s) requests
      //--------------------------------------------------------------
      class CCurlppHttpRestRequest final : public IHttpRestRequest
      {
      public:
         CCurlppHttpRestRequest(EType requestType,
                                std::string url);

         ~CCurlppHttpRestRequest() override = default;

         // IHttpRestRequest implementation
         IHttpRestRequest& withHeaderParameters(const std::map<std::string, std::string>& headerParameters) override;
         IHttpRestRequest& withParameters(const std::map<std::string, std::string>& parameters) override;
         IHttpRestRequest& withTimeout(int timeoutSeconds) override;
         IHttpRestRequest& withBody(const std::string& body) override;
         IHttpRestRequest& withBasicAuthentication(const std::string& user,
                                                   const std::string& password) override;

         void send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                            const std::string& data)>& responseHandlerFct) override;

         void send(const std::function<void(boost::shared_ptr<CDataContainer> data)>& responseHandlerFct) override;
         std::string send() override;
         // [END] IHttpRestRequest implementation

      private:
         std::map<std::string, std::string> formatResponseHeaders(const std::string& headersBuffer) const;
         boost::shared_ptr<CDataContainer> processJsonResponse(const std::map<std::string, std::string>& receivedHeaders,
                                                               const std::string& data) const;

         curlpp::Easy m_request;
         EType m_requestType;
         const std::string m_url;
         int m_timeoutSeconds;
         std::map<std::string, std::string> m_headerParameters;
         std::map<std::string, std::string> m_parameters;
         std::string m_body;
      };
   }
} // namespace shared::http
