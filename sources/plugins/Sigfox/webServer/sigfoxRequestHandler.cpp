#include "stdafx.h"
#include "sigfoxRequestHandler.h"
#include <shared/Log.h>
#include <shared/DataContainer.h>

CSigfoxRequestHandler::CSigfoxRequestHandler(shared::event::CEventHandler& receiveDataEventHandler,
                                             int receiveDataEventId):
   m_receiveDataEventHandler(receiveDataEventHandler),
   m_receiveDataEventId(receiveDataEventId)
{}

void CSigfoxRequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
{
   resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
   resp.setContentType("text/html");

   YADOMS_LOG(information) << "Receive something";

   if (boost::icontains(req.getContentType(), "application/json"))
   {
      YADOMS_LOG(information) << "Receive a json file";

      std::istream &i = req.stream();
      int len = req.getContentLength();
      char* buffer = new char[len];
      i.read(buffer, len);

      YADOMS_LOG(information) << buffer;

      m_receiveDataEventHandler.postEvent<shared::CDataContainer>(m_receiveDataEventId,
                                                                  shared::CDataContainer(buffer));
   }

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