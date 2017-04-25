#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"
#include <shared/Log.h>

shared::CDataContainer urlManager::readFileState(Poco::Net::SocketAddress socket,
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

shared::CDataContainer urlManager::setRelayState(Poco::Net::SocketAddress socket,
                                                 const shared::CDataContainer& credentials,
                                                 const shared::CDataContainer& parameters)
{
   std::stringstream url;

   // create the URL
   url << "http://" << socket.toString() << "RL.cgi";

   credentials.printToLog(YADOMS_LOG(information));
   YADOMS_LOG(information) << url.str();

   return http::CHttpMethods::SendGetRequest(url.str(), credentials, parameters);
}

//http://WES/RL.cgi?rl1=ON&rl2=OFF