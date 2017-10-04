#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>


namespace web { namespace poco {
      
   /*
    * This class avoids to recreate a new request handler for each web request.
    * It's inherits from HTTPRequestHandler and contains a SharedPtr on HTTPRequestHandler.
    * So each web request will create only a container, not the full object.
    */
   class CHttpRequestHandlerContainer : public Poco::Net::HTTPRequestHandler
   {
   public:
      /**
       * Ctor
       * @param m_requestHandler : HTTP handler to encapsulate
       */
      explicit CHttpRequestHandlerContainer(boost::shared_ptr<Poco::Net::HTTPRequestHandler> & m_requestHandler, bool allowExternalAccess);

      /**
       * Dtor
       */
      virtual ~CHttpRequestHandlerContainer();

      /**
       * Process the request
       * @param request : request to process
       * @param response : answer to send back
       */
      virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

   private:
      boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_requestHandler;
      bool m_allowExternalAccess;
   };
   
} //namespace poco
} //namespace web


