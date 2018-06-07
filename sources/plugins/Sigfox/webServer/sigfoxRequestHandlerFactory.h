#pragma once
#include <shared/event/EventHandler.hpp>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

//--------------------------------------------------------------
/// \brief	Configuration of the plugin
//--------------------------------------------------------------
class CSigfoxRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
   CSigfoxRequestHandlerFactory(shared::event::CEventHandler& receiveDataEventHandler,
                                int receiveDataEventId);
   virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &);

private:
   shared::event::CEventHandler& m_receiveDataEventHandler;
   int m_receiveDataEventId;
};