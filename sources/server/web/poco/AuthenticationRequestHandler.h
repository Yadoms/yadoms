#pragma once
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerResponse.h>
#include "authentication/IAuthentication.h"

namespace web
{
   namespace poco
   {
      //-------------------------------------
      ///\brief Authentication request handler. This is an authentication layer which check for authorized only users requests
      //-------------------------------------
      class CAuthenticationRequestHandler : public Poco::Net::HTTPRequestHandler
      {
      public:
         //-------------------------------------
         ///\brief Constructor
         ///\param [in]    authenticator        The authenticator object which manage authentication
         ///\param [in]    baseRequestHandler   handle requests called after a successfull authentication
         ///\param [in]    allowAuthentication  Indicate if the current request allow authentication (web only, should be false for rest and ws handlers)
         //-------------------------------------
         CAuthenticationRequestHandler(boost::shared_ptr<authentication::IAuthentication> authenticator, boost::shared_ptr<Poco::Net::HTTPRequestHandler> baseRequestHandler, bool allowAuthentication);

         //-------------------------------------
         ///\brief Destructor
         //-------------------------------------
         virtual ~CAuthenticationRequestHandler();


         // Poco::Net::HTTPRequestHandler implementation
         void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
         // [END] Poco::Net::HTTPRequestHandler implementation

      private:
         //-------------------------------------
         ///\brief Authenticator object
         //-------------------------------------
         boost::shared_ptr<authentication::IAuthentication> m_authenticator;

         //-------------------------------------
         ///\brief Base request handler
         //-------------------------------------
         boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_baseRequestHandler;

         //-------------------------------------
         ///\brief Allow the request to authenticate (web only, should be false for rest and ws)
         //-------------------------------------
         bool m_bAllowAuthentication;
      };
   } //namespace poco
} //namespace web


