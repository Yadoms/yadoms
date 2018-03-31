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
   //TODO : Check the URI name should finish by /sigfox
   YADOMS_LOG(trace) << "getURI : " << req.getURI();

   // Separate key/value
   boost::char_separator<char> sep("/");
   std::string value = req.getURI();
   boost::tokenizer<boost::char_separator<char>> tok( value, sep);

   auto iterator = tok.begin();
   if (iterator != tok.end())
   {
      const auto key = *iterator;
      ++iterator;

      if (iterator != tok.end())
      {
         const auto value = *iterator;
         YADOMS_LOG(information) << "token : " << value;
      }
   }

   if (req.getMethod() == "POST")
   {
      if (boost::icontains(req.getContentType(), "application/json"))
      {
         YADOMS_LOG(trace) << "Receive a json file";

         std::istream &i = req.stream();
         int len = req.getContentLength();
         char* buffer = new char[len];
         i.read(buffer, len);

         YADOMS_LOG(trace) << buffer;

         m_receiveDataEventHandler.postEvent<shared::CDataContainer>(m_receiveDataEventId,
                                                                     shared::CDataContainer(buffer));
      }
   }

   // Send the response
   resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
   resp.setContentType("text/html");
   resp.send();
}