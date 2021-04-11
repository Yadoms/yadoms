#pragma once
#include <map>
#include <functional>

namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	HTTP(s) requests handling interface
      /// \note To better user-experience, functions are stackable.
      /// Call example :
      /// request
      ///    .withHeaderParameters({{"param1", "value1"}, {"param2", "value2"}})
      ///    .withTimeout(30)
      ///    .withBasicAuthentication("user", "passwd")
      ///    .send();
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
         
         //--------------------------------------------------------------
         /// Functions to build the request
         //--------------------------------------------------------------
         virtual IHttpRestRequest& withHeaderParameters(const std::map<std::string, std::string>& headerParameters) = 0;
         virtual IHttpRestRequest& withParameters(const std::map<std::string, std::string>& parameters) = 0;
         virtual IHttpRestRequest& withTimeout(int timeoutSeconds) = 0;
         virtual IHttpRestRequest& withBody(const std::string& body) = 0;
         virtual IHttpRestRequest& withBasicAuthentication(const std::string& user,
                                                           const std::string& password) = 0;


         //--------------------------------------------------------------
         /// Function to send the built request
         //--------------------------------------------------------------
         
         //--------------------------------------------------------------
         /// \brief	Send the built request, and wait for a response with data as string and opportunity to read received header
         /// \param responseHandlerFct A lambda which received response headers and data as string
         //--------------------------------------------------------------
         virtual void send(const std::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                                    const std::string& data)>& responseHandlerFct) = 0;

         //--------------------------------------------------------------
         /// \brief	Send the built request, and wait for a response with JSON data as CDataContainer
         /// \param responseHandlerFct A lambda which received response data as CDataContainer
         //--------------------------------------------------------------
         virtual void send(const std::function<void(boost::shared_ptr<CDataContainer> data)>& responseHandlerFct) = 0;

         //--------------------------------------------------------------
         /// \brief	Send the built request, and wait for a response with data as string
         /// \return The received response data
         //--------------------------------------------------------------
         virtual std::string send() = 0;
      };
   }
} // namespace shared::http
