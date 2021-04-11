#pragma once
#include <shared/DataContainer.h>

#include "IHttpRestRequest.h"


namespace shared
{
   namespace http
   {
      //--------------------------------------------------------------
      /// \brief	Helpers fot simple HTTP(s) REST requests
      ///
      /// If you need more request customization, call createHttpRestRequest and use directly IHttpRestRequest object
      ///
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
         /// \brief	    Send GET request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send GET request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonGetRequest(
            const std::string& url,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());


         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // POST
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPostRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send POST request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPostRequest(
            const std::string& url,
            const std::string& body,
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
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the received headers
         //--------------------------------------------------------------
         static std::map<std::string, std::string> sendHeadRequest(
            const std::string& url,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         //--------------------------------------------------------------
         // PUT
         //--------------------------------------------------------------
         //--------------------------------------------------------------

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static std::string sendPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());

         //--------------------------------------------------------------
         /// \brief	    Send PUT request to remote server (for JSON answer)
         /// \param[in]  url                 the url to send the request
         /// \param[in]  body                The request content
         /// \param[in]  parameters          parameters at the end of the url
         /// \return     the answer of the request
         //--------------------------------------------------------------
         static boost::shared_ptr<CDataContainer> sendJsonPutRequest(
            const std::string& url,
            const std::string& body,
            const std::map<std::string, std::string>& parameters = std::map<std::string, std::string>());
      };
   }
} // namespace shared::http
