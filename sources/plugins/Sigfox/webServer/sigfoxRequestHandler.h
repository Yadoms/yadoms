#pragma once
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSigfoxRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
   CSigfoxRequestHandler(shared::event::CEventHandler& receiveDataEventHandler,
                         int receiveDataEventId);
   virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);

private:

   //--------------------------------------------------------------
   /// \brief	The event handler to notify for received data event   
   //--------------------------------------------------------------
   shared::event::CEventHandler& m_receiveDataEventHandler;
   int m_receiveDataEventId;
};