#pragma once
#include <map>
#include <functional>

namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	HTTP(s) requests handling interface
      //--------------------------------------------------------------
      class IHttpRestRequest
      {
      public:
         virtual ~IHttpRestRequest() = default;

         //--------------------------------------------------------------
         /// \brief	default value for HTTP Request default timeout
         //--------------------------------------------------------------
         static const int HttpRequestDefaultTimeoutSeconds = 45;

         //--------------------------------------------------------------
         /// \brief	REST Request type
         //--------------------------------------------------------------
         enum class EType
         {
            kGet = 0,
            kPut,
            kPost,
            kHead
         };

         virtual IHttpRestRequest& withHeaderParameters(const std::map<std::string, std::string>& headerParameters) = 0;
         virtual IHttpRestRequest& withParameters(const std::map<std::string, std::string>& parameters) = 0;
         virtual IHttpRestRequest& withTimeout(int timeoutSeconds) = 0;
         virtual IHttpRestRequest& withBody(const std::string& body) = 0;
         virtual IHttpRestRequest& withBasicAuthentication(const std::string& user,
                                                           const std::string& password) = 0;

         virtual void send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                    const std::string& data)>& responseHandlerFct) = 0;

         virtual void send(const std::function<void(boost::shared_ptr<CDataContainer> data)>& responseHandlerFct) = 0;
      };
   }
} // namespace shared::http
