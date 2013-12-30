#pragma once

#include "web/IWebServer.h"
#include "cWebem.h"
#include "web/rest/IRestService.h"

class CWebServer : public IWebServer
{
public:
   //
   //\brief              Create aa embedded w<eb server
   //\param[in] address  IP address.  In general, use  "0.0.0.0"
   //\param[in] port     port to listen on for browser requests e.g. "8080"
   //\param[in] doc_root path to folder containing html e.g. "./"
   //
   CWebServer(const std::string & address, const std::string & port, const std::string & doc_root );

   //
   //\brief Descturtor
   //
   virtual ~CWebServer();

   // IWebServer implementation
   virtual void start();
   virtual void stop();
   virtual void configureRestService(boost::shared_ptr<IRestService> restService);
   // [END] IWebServer implementation


private:
   //--------------------------------------   
   ///\brief  Method which handle rest requests
   //--------------------------------------   
   std::string manageRestRequests(const http::server::request & request);

private:
   boost::shared_ptr<http::server::cWebem> m_embeddedWebServer;

   std::string m_configAddress;
   std::string m_configPort;
   std::string m_configDocRoot;

   std::string m_restBaseKeyword;
   std::map<std::string , boost::shared_ptr<IRestService> > m_restServices;

};