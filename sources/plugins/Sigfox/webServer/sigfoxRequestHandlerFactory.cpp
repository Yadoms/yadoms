#include "stdafx.h"
#include "sigfoxRequestHandlerFactory.h"
#include "sigfoxRequestHandler.h"

Poco::Net::HTTPRequestHandler* CSigfoxRequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &)
{
   return new CSigfoxRequestHandler();
}