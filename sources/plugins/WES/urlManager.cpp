#include "stdafx.h"
#include "urlManager.h"
#include "shared/http/HttpMethods.h"

shared::CDataContainer urlManager::sendCommand(Poco::Net::SocketAddress socket,
                                               std::string &user,
                                               std::string &password,
                                               std::string &file)
{
   std::stringstream url;

   //TODO : How to integrate the user and the password ?

   // create the URL
   url << "http://" << socket.toString() << "ASSETS/CGX/YADOMS/" + file;

   return shared::CHttpMethods::SendGetRequest(url.str()/*, parameters*/);
}