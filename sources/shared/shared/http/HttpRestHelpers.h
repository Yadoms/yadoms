#pragma once
#include <shared/DataContainer.h>

#include "IHttpRestRequest.h"


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot HTTP(s) REST requests
      //--------------------------------------------------------------
      class CHttpRestHelpers final
      {
      public:
         CHttpRestHelpers() = delete;
         ~CHttpRestHelpers() = default;

         static boost::shared_ptr<IHttpRestRequest> createHttpRestRequest(IHttpRestRequest::EType requestType,
                                                                          const std::string& url);


         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // GET
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server with response processing injection
         /// \param[in]  url                 the url to send the request
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendGetRequest(
            const std::string& url,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                       const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server with response processing injection with basic Authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendGetRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& user,
            const std::string& password,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                       const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server with basic Authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendGetRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server (for JSON answer) with basic Authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonGetRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // POST
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server with response processing injection
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                the body of request
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendPostRequest(
            const std::string& url,
            const std::string& body,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders,
               const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPostRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server with response processing injection and basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                the body of request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendPostRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders,
               const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server using basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPostRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPostRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<
               std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<
               std::string, std::string>());


         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server (for JSON answer) using basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPostRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<
               std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<
               std::string, std::string>());

         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // HEAD
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         //--------------------------------------------------------------
         /// \brief	    Send HEAD request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendHeadRequest(
            const std::string& url,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send HEAD request to remote server using basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendHeadRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& user,
            const std::string& password,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send HEAD request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the received headers
         //--------------------------------------------------------------
         static std::map<std::string, std::string> sendHeadRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send HEAD request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the received headers
         //--------------------------------------------------------------
         static std::map<std::string, std::string> sendHeadRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // PUT
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         /// \brief	    Send PUT request to remote server with response processing injection
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                the body of request
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendPutRequest(
            const std::string& url,
            const std::string& body,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                       const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         /// \brief	    Send PUT request to remote server with response processing injection and Basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                the body of request
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  responseHandlerFct  lambda for response processing
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         //--------------------------------------------------------------
         static void sendPutRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const boost::function<void(const std::map<std::string, std::string>& receivedHeaders,
                                       const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server with basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPutRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server (for JSON answer) with basic authentication
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  user                the username
         /// \param[in]  password            user password
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPutRequestWithBasicAuthentication(
            const std::string& url,
            const std::string& body,
            const std::string& user,
            const std::string& password,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());
      };
   }
} // namespace shared::http
