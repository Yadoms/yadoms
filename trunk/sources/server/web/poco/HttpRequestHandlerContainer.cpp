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

      m_requestHandler->handleRequest(request, response);
   }

} //namespace poco
} //namespace web

   