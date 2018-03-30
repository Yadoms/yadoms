#include "stdafx.h"
#include "sigfoxRequestHandler.h"
#include <shared/Log.h>

/*
CSigfoxRequestHandler::CSigfoxRequestHandler()
{

}*/

void CSigfoxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
   resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
   resp.setContentType("text/html");

   YADOMS_LOG(information) << "Receive something";

   if (boost::icontains(req.getContentType(), "application/json"))
   {
      YADOMS_LOG(information) << "Receive a json file";
   }

   //api->getEventHandler().createTimer(kEvtInitialization, shared::event::CEventTimer::kOneShot, boost::posix_time::seconds(0));

   /*
   std::ostream& out = resp.send();
   
   out << "<h1>Hello world!</h1>"
      << "<p>Count: " << ++std::count << "</p>"
      << "<p>Host: " << req.getHost() << "</p>"
      << "<p>Method: " << req.getMethod() << "</p>"
      << "<p>URI: " << req.getURI() << "</p>";
   out.flush();

   std::cout << std::endl
      << "Response sent for count=" << std::count
      << " and URI=" << req.getURI() << std::endl;
      */
}