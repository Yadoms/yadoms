#pragma once

#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"


namespace web { namespace poco {

   class CWebSocketRequestHandler : public Poco::Net::HTTPRequestHandler
   {
   public:
      void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);
   };


} //namespace poco
} //namespace web

