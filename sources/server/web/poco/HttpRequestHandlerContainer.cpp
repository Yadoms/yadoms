#include "stdafx.h"
#include "HttpRequestHandlerContainer.h"
#include <shared/exception/NullReference.hpp>

namespace web { namespace poco {
      
   CHttpRequestHandlerContainer::CHttpRequestHandlerContainer(boost::shared_ptr<Poco::Net::HTTPRequestHandler> & requestHandler)
   : m_requestHandler(requestHandler)
   {
   }

   CHttpRequestHandlerContainer::~CHttpRequestHandlerContainer()
   {
   }

   void CHttpRequestHandlerContainer::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
   {
      if (!m_requestHandler)
         throw shared::exception::CNullReference("requestHandler must be defined in CHttpRequestHandlerContainer");

      response.add("Access-Control-Allow-Origin", "*");
      response.add("Access-Control-Allow-Credentials", "true");
      response.add("Access-Control-Allow-Methods", "GET, HEAD, OPTIONS, POST, PUT, DELETE");
      response.add("Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Access-Control-Allow-Methods, Origin,Accept, X-Requested-With, Content-Type, Access-Control-Allow-Origin, Access-Control-Request-Methods, Access-Control-Request-Method, Access-Control-Request-Headers");

      m_requestHandler->handleRequest(request, response);
   }

} //namespace poco
} //namespace web

   