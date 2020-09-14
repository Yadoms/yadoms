#include "stdafx.h"
#include "urlManager.h"
#include <shared/Log.h>

boost::posix_time::time_duration urlManager::httpRequestCreationTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(5)));
boost::posix_time::time_duration urlManager::httpRequestWESTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(25)));

boost::shared_ptr<shared::CDataContainer> urlManager::readFileState(
	Poco::Net::SocketAddress socket,
    const boost::shared_ptr<shared::CDataContainer>& credentials,
    const std::string &file,
	http::httpContext& context,
    const boost::posix_time::time_duration& timeout)
{
   std::stringstream url;
   boost::shared_ptr<shared::CDataContainer> noParameters = shared::CDataContainer::make();

   // create the URL
   url << "http://" << socket.toString() << "/WEBPROG/CGX/YADOMS/" + file;
   YADOMS_LOG(trace) << "URL : " << url.str();
   boost::shared_ptr<shared::CDataContainer> response =  http::CHttpMethods::SendGetRequest(url.str(), credentials,  noParameters, context, timeout);
   return response;
}

boost::shared_ptr<shared::CDataContainer> urlManager::setRelayState(
	Poco::Net::SocketAddress socket,
    const boost::shared_ptr<shared::CDataContainer>& credentials,
    const boost::shared_ptr<shared::CDataContainer>& parameters,
	http::httpContext& context)
{
   std::stringstream url;
   boost::shared_ptr<shared::CDataContainer> response = shared::CDataContainer::make();

   // create the URL
   url << "http://" << socket.toString() << "/RL.cgx";
   YADOMS_LOG(trace) << "URL : " << url.str();

   boost::shared_ptr<shared::CDataContainer> responseTree = http::CHttpMethods::SendGetRequest(
	   url.str(),
       credentials, 
       parameters,
	   context,
       httpRequestWESTimeout);

   response->set("Relai1", responseTree->get<std::string>("data.relais.RELAIS1"));
   response->set("Relai2", responseTree->get<std::string>("data.relais.RELAIS2"));
   response->printToLog(YADOMS_LOG(trace));

   return response;
}