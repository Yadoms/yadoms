#include "stdafx.h"
#include "HttpRequestHandlerContainer.h"
#include <shared/exception/NullReference.hpp>

namespace web { namespace poco {
      
   CHttpRequestHandlerContainer::CHttpRequestHandlerContainer(boost::shared_ptr<Poco::Net::HTTPRequestHandler> & requestHandler, bool allowExternalAccess)
   : m_requestHandler(requestHandler), m_allowExternalAccess(allowExternalAccess)
   {
   }

   CHttpRequestHandlerContainer::~CHttpRequestHandlerContainer()
   {
   }

   void CHttpRequestHandlerContainer::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
   {
      if (!m_requestHandler)
         throw shared::exception::CNullReference("requestHandler must be defined in CHttpRequestHandlerContainer");

      if (m_allowExternalAccess)
      {
         //Add CORS headers (Cross-origin resource sharing)
         //If headers are not added, then only a website/app from the same server is allowed (http server and rest/ws server must be the same).
         //For mobile app, the files are statically embedded in app (so without httpserver) and they must access an external yadoms instance for resT/ws requests.
         response.add("Access-Control-Allow-Origin", "*");
         response.add("Access-Control-Allow-Credentials", "true");
         response.add("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS, POST, PUT, DELETE");
         response.add("Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Access-Control-Allow-Methods, Origin,Accept, X-Requested-With, Content-Type, Access-Control-Allow-Origin, Access-Control-Request-Methods, Access-Control-Request-Method, Access-Control-Request-Headers");
      }

      m_requestHandler->handleRequest(request, response);
   }

} //namespace poco
} //namespace web

   