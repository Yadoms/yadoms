#include "stdafx.h"
#include "AuthenticationRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include "MimeType.h"


namespace web { namespace poco {

      CAuthenticationRequestHandler::CAuthenticationRequestHandler(boost::shared_ptr<authentication::IAuthentication> authenticator, boost::shared_ptr<Poco::Net::HTTPRequestHandler> baseRequestHandler)
         :m_authenticator(authenticator), m_baseRequestHandler(baseRequestHandler)
      {

      }

      CAuthenticationRequestHandler::~CAuthenticationRequestHandler()
      {

      }

      void CAuthenticationRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         //check if request contains credentials
         if (!request.hasCredentials())
         {
            //there are no credentials data in request, return unauthorized http (401)
            response.requireAuthentication("Yadoms");
            response.setContentLength(0);
            response.send();
         }
         else
         {
            //the request contains credentials data, just ensure it is valid
            Poco::Net::HTTPBasicCredentials cred(request);
            const std::string& user = cred.getUsername();
            const std::string& pwd = cred.getPassword();
            if (!m_authenticator->authenticate(user, pwd))
            {
               //credentials are no more valid, eturn unauthorized http (401)
               response.requireAuthentication("Yadoms");
               response.send();
            }
            else
            {
               //authentication is valid, process request handler
               m_baseRequestHandler->handleRequest(request, response);
            }
         }
      }
} //namespace poco
} //namespace web

