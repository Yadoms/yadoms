#include "stdafx.h"
#include "sigfoxRequestHandlerFactory.h"
#include "sigfoxRequestHandler.h"

CSigfoxRequestHandlerFactory::CSigfoxRequestHandlerFactory(shared::event::CEventHandler& receiveDataEventHandler,
                                                           int receiveDataEventId):
   m_receiveDataEventHandler(receiveDataEventHandler),
   m_receiveDataEventId(receiveDataEventId)
{}

Poco::Net::HTTPRequestHandler* CSigfoxRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &)
{
   return new CSigfoxRequestHandler(m_receiveDataEventHandler, m_receiveDataEventId);
}