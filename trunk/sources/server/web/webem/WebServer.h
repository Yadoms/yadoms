#pragma once

#include "web/IWebServer.h"
#include "cWebem.h"
#include "web/rest/IRestService.h"
#include "RestHandler.h"

//
//\brief An embedded web server which supports REST api
//
class CWebServer : public IWebServer
{
public:
   //
   //\brief              Create aa embedded w<eb server
   //\param[in] address  IP address.  In general, use  "0.0.0.0"
   //\param[in] port     port to listen on for browser requests e.g. "8080"
   //\param[in] doc_root path to folder containing html e.g. "./"
   //\param[in] restKeywordBase the string which identifies a rest url ex: /rest/
   //
   CWebServer(const std::string & address, const std::string & port, const std::string & doc_root, const std::string & restKeywordBase );

   //
   //\brief Descturtor
   //
   virtual ~CWebServer();

   // IWebServer implementation
   virtual void start();
   virtual void stop();
   virtual boost::shared_ptr<IRestHandler> getRestHandler();
   virtual void configureAlias(const std::string & alias, const std::string & path);
   // [END] IWebServer implementation


private:
   std::string m_configAddress;
   std::string m_configPort;
   std::string m_configDocRoot;
   boost::shared_ptr<CRestHandler> m_restHandler;
   boost::shared_ptr<http::server::cWebem> m_embeddedWebServer;
};