#pragma once
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include "IAuthentication.h"

namespace web
{
   namespace poco
   {
      //-------------------------------------
      ///\brief Authentication request handler. This is an authentication layer which check for authorized only users requests
      //-------------------------------------
      class CAuthenticationRequestHandler final : public Poco::Net::HTTPRequestHandler
      {
      public:
         //-------------------------------------
         ///\brief Constructor
         ///\param [in]    authenticator        The authenticator object which manage authentication
         ///\param [in]    baseRequestHandler   handle requests called after a successfull authentication
         ///\param [in]    allowAuthentication  Indicate if the current request allow authentication
         //-------------------------------------
         CAuthenticationRequestHandler(boost::shared_ptr<IAuthentication> authenticator,
                                       boost::shared_ptr<HTTPRequestHandler> baseRequestHandler,
                                       bool allowAuthentication);

         //-------------------------------------
         ///\brief Destructor
         //-------------------------------------
         ~CAuthenticationRequestHandler() override;


         // Poco::Net::HTTPRequestHandler implementation
         void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response) override;
         // [END] Poco::Net::HTTPRequestHandler implementation

      private:
         //-------------------------------------
         ///\brief Authenticator object
         //-------------------------------------
         boost::shared_ptr<IAuthentication> m_authenticator;

         //-------------------------------------
         ///\brief Base request handler
         //-------------------------------------
         boost::shared_ptr<HTTPRequestHandler> m_baseRequestHandler;

         //-------------------------------------
         ///\brief Allow the request to authenticate (web only, should be false for rest and ws)
         //-------------------------------------
         bool m_bAllowAuthentication;
      };
   } //namespace poco
} //namespace web
