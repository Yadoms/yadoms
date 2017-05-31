#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"
#include <shared/Log.h>

static boost::posix_time::time_duration httpRequestWESTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(8)));

void urlManager::parseNode(shared::CDataContainer &container, boost::property_tree::ptree node)
{
   boost::property_tree::ptree::const_iterator end = node.end();
   std::string attributeName;
   std::string attributeValue;

   for (boost::property_tree::ptree::const_iterator it = node.begin(); it != end; ++it)
   {
      if (it->second.size() != 0)
      {
         shared::CDataContainer subNode;
         parseNode(container, it->second);
      }
      else
      {
         if (it->first == "id" || it->first == "var")
            attributeName = it->second.data();

         if (it->first == "value")
            attributeValue = it->second.data();
      }
   }

   container.set(attributeName, attributeValue);
}

shared::CDataContainer urlManager::readFileState(Poco::Net::SocketAddress socket,
                                                 const shared::CDataContainer& credentials,
                                                 const std::string &file)
{
   std::stringstream url;
   shared::CDataContainer noParameters;
   shared::CDataContainer response;
   boost::property_tree::ptree responseTree;

   // create the URL
   url << "http://" << socket.toString() << "/ASSETS/CGX/YADOMS/" + file;
   YADOMS_LOG(trace) << "URL : " << url.str();

   responseTree =  http::CHttpMethods::SendGetRequest(url.str(),
                                                      credentials, 
                                                      noParameters,
                                                      httpRequestWESTimeout);
   parseNode(response, responseTree);
   return response;
}

shared::CDataContainer urlManager::setRelayState(Poco::Net::SocketAddress socket,
                                                 const shared::CDataContainer& credentials,
                                                 const shared::CDataContainer& parameters)
{
   std::stringstream url;
   shared::CDataContainer response;
   boost::property_tree::ptree responseTree;

   // create the URL
   url << "http://" << socket.toString() << "/RL.cgx";
   YADOMS_LOG(trace) << "URL : " << url.str();

   responseTree = http::CHttpMethods::SendGetRequest(url.str(), 
                                                     credentials, 
                                                     parameters,
                                                     httpRequestWESTimeout);

   response.set("Relai1", responseTree.get_child("data").get_child("relais").get_child("RELAIS1").data());
   response.set("Relai2", responseTree.get_child("data").get_child("relais").get_child("RELAIS2").data());
   response.printToLog(YADOMS_LOG(trace));

   return response;
}