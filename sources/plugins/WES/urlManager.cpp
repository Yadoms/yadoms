#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"
#include <shared/Log.h>

shared::CDataContainer urlManager::sendCommand(Poco::Net::SocketAddress socket,
                                               const shared::CDataContainer& credentials,
                                               const std::string &file)
{
   std::stringstream url;

   shared::CDataContainer parameters;
   // create the URL
   url << "http://" << socket.toString() << "/ASSETS/CGX/YADOMS/" + file;

   credentials.printToLog(YADOMS_LOG(information));
   YADOMS_LOG(information) << url.str();

   return http::CHttpMethods::SendGetRequest(url.str(), credentials, parameters);
}