#pragma once
#include <shared/DataContainer.h>
#include <boost/property_tree/exceptions.hpp>


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot HTTP(s) requests
      //--------------------------------------------------------------
      class CHttpMethods final
      {
      public:
         //--------------------------------------------------------------
         /// \brief	default value for HTTP Request default timeout
         //--------------------------------------------------------------
         static const int HttpRequestDefaultTimeoutSeconds = 45;

         CHttpMethods() = delete;
         ~CHttpMethods() = default;


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
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static void sendGetRequest(
            const std::string& url,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders,
               const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);


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
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static void sendPostRequest(
            const std::string& url,
            const std::string& body,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders,
               const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPostRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPostRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);


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
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static void sendHeadRequest(
            const std::string& url,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send HEAD request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the received headers
         //--------------------------------------------------------------
         static std::map<std::string, std::string> sendHeadRequest(
            const std::string& url,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

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
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static void sendPutRequest(
            const std::string& url,
            const std::string& body,
            const boost::function<void(
               const std::map<std::string, std::string>& receivedHeaders,
               const std::string& data)>& responseHandlerFct,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      Timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  headerParameters    parameters included into the frame
         /// \param[in]  parameters          parameters at the end of the url
         /// \param[in]  timeoutSeconds      timeout for the request (seconds)
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& headerParameters = std::map<std::string, std::string>(),
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>(),
            int timeoutSeconds = HttpRequestDefaultTimeoutSeconds);

      protected:
         static boost::shared_ptr<CDataContainer> processJsonResponse(
            const std::map<std::string, std::string>& receivedHeaders,
            const std::string& data);
      };
   }
} // namespace shared::http
