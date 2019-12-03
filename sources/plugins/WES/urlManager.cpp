#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));
boost::posix_time::time_duration urlManager::httpRequestWESTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));



shared::CDataContainer urlManager::readFileState(Poco::Net::SocketAddress socket,
                                                 const shared::CDataContainer& credentials,
                                                 const std::string &file,
                                                 const boost::posix_time::time_duration& timeout)
{
   std::stringstream url;
   shared::CDataContainer noParameters;
   shared::CDataContainer response;

   // create the URL
   url << "http://" << socket.toString() << "/WEBPROG/CGX/YADOMS/" + file;
   YADOMS_LOG(trace) << "URL : " << url.str();
   response =  http::CHttpMethods::SendGetRequest(url.str(), credentials,  noParameters, timeout);
   return response;
}

shared::CDataContainer urlManager::setRelayState(Poco::Net::SocketAddress socket,
                                                 const shared::CDataContainer& credentials,
                                                 const shared::CDataContainer& parameters)
{
   std::stringstream url;
   shared::CDataContainer response;

   // create the URL
   url << "http://" << socket.toString() << "/RL.cgx";
   YADOMS_LOG(trace) << "URL : " << url.str();

   shared::CDataContainer responseTree = http::CHttpMethods::SendGetRequest(url.str(),
                                                     credentials, 
                                                     parameters,
                                                     httpRequestWESTimeout);

   response.set("Relai1", responseTree.get<std::string>("data.relais.RELAIS1"));
   response.set("Relai2", responseTree.get<std::string>("data.relais.RELAIS2"));
   response.printToLog(YADOMS_LOG(trace));

   return response;
}