#include "stdafx.h"
#include "AuthenticationRequestHandler.h"
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPBasicCredentials.h>


namespace web
{
   namespace poco
   {
      CAuthenticationRequestHandler::CAuthenticationRequestHandler(boost::shared_ptr<authentication::IAuthentication> authenticator, boost::shared_ptr<HTTPRequestHandler> baseRequestHandler, bool allowAuthentication)
         : m_authenticator(authenticator), m_baseRequestHandler(baseRequestHandler), m_bAllowAuthentication(allowAuthentication)
      {
      }

      CAuthenticationRequestHandler::~CAuthenticationRequestHandler()
      {
      }

      void CAuthenticationRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         if (m_authenticator && m_authenticator->isAuthenticationActive())
         {
            //check if request contains credentials
            if (!request.hasCredentials())
            {
               if (m_bAllowAuthentication)
               {
                  //there are no credentials data in request, return unauthorized http (401)
                  response.requireAuthentication("Yadoms");
                  response.setContentLength(0);
                  response.send();
               }
               else
               {
                  //there are no credentials data in request, return forbidden (403) because this http handler do not allow authentication
                  response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_FORBIDDEN, "Please login first");
                  response.setContentLength(0);
                  response.send();
               }
            }
            else
            {
               //the request contains credentials data, just ensure it is valid
               Poco::Net::HTTPBasicCredentials cred(request);
               const auto& user = cred.getUsername();
               const auto& pwd = cred.getPassword();
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
         else
         {
            //no authentication required
            m_baseRequestHandler->handleRequest(request, response);
         }
      }
   } //namespace poco
} //namespace web


