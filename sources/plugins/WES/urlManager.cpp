#include "stdafx.h"
#include "urlManager.h"
#include "http/HttpMethods.h"
#include <shared/Log.h>

void urlManager::parseNode(shared::CDataContainer &container, boost::property_tree::ptree node)
{
   boost::property_tree::ptree::const_iterator end = node.end();
   std::string attributeName;
   std::string attributeValue;

   for (boost::property_tree::ptree::const_iterator it = node.begin(); it != end; ++it)
   {
      //YADOMS_LOG(information) << "it->first : " << it->first;
      //YADOMS_LOG(information) << "it->second.data() : " << it->second.data();

      if (it->second.size() != 0)
      {
         shared::CDataContainer subNode;
         parseNode(container/*subNode*/, it->second);
         //container.set(it->first, subNode);
      }
      else
      {
         if (it->first == "id" || it->first == "var")
            attributeName = it->second.data();

         if (it->first == "value")
            attributeValue = it->second.data();
         //container.set(it->first, it->second.data());
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

   credentials.printToLog(YADOMS_LOG(information));
   YADOMS_LOG(information) << url.str();

   responseTree =  http::CHttpMethods::SendGetRequest(url.str(),
                                                      credentials, 
                                                      noParameters);
   parseNode(response, responseTree);
   response.printToLog(YADOMS_LOG(information));
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
   url << "http://" << socket.toString() << "RL.cgi";

   credentials.printToLog(YADOMS_LOG(information));
   YADOMS_LOG(information) << url.str();

   responseTree = http::CHttpMethods::SendGetRequest(url.str(), credentials, parameters);

   // TODO : Analyze response

   return response;
}

//http://WES/RL.cgi?rl1=ON&rl2=OFF