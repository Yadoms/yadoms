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
   // Separate key/value
   boost::char_separator<char> sep("/");
   std::string value = req.getURI();
   boost::tokenizer<boost::char_separator<char>> tok( value, sep);

   auto iterator = tok.begin();
   std::vector<std::string> token;
   if (iterator != tok.end())
   {
      token.push_back(*iterator);
      ++iterator;
   }

   // We accept only 1 token with sigfox
   if (req.getMethod() == "POST" && token.size() == 1 && token[0] == "sigfox")
   {
      if (boost::icontains(req.getContentType(), "application/json"))
      {
         auto& i = req.stream();
         const auto len = req.getContentLength();
         const auto buffer = new char[len];
         i.read(buffer, len);

         m_receiveDataEventHandler.postEvent<shared::CDataContainer>(m_receiveDataEventId,
                                                                     shared::CDataContainer(buffer));
      }
      else
      {
         YADOMS_LOG(information) << "The content type is " << req.getContentType() << " and it could not be parsed";
      }
   }
   else
   {
      if (req.getMethod() != "POST") 
         YADOMS_LOG(information) << "Method " << req.getContentType() << "is not treated";
      if (token.size() != 1 || token[0] != "sigfox")
      {
         YADOMS_LOG(information) << "URI " << req.getURI() << "is not correct";
         YADOMS_LOG(information) << "URI should be /sigfox";
      }
   }

   // Send the response
   resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
   resp.setContentType("text/html");
   resp.send();
}