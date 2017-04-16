#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"

shared::CDataContainer urlManager::sendCommand(Poco::Net::SocketAddress socket,
                                               const std::string &user,
                                               const std::string &password,
                                               const std::string &file)
{
   std::stringstream url;

   shared::CDataContainer parameters;

   // create the URL
   //url << "http://" << socket.toString() << "/ASSETS/CGX/YADOMS/" + file;
   url << "http://192.168.1.37/ASSETS/CGX/YADOMS/" + file; //admin:wes@

   return http::CHttpMethods::SendGetRequest(url.str(), parameters);
}