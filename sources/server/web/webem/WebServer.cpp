#include "stdafx.h"
#include "WebServer.h"
#include "tools/Log.h"

CWebServer::CWebServer(	   const std::string & address, const std::string & port, const std::string & doc_root )
   :m_configAddress(address), m_configPort(port), m_configDocRoot(doc_root)
{
   m_restHandler.reset(new CRestHandler("/rest/"));
}

CWebServer::~CWebServer()
{
   stop();
}


// IWebServer implementation
void CWebServer::start()
{
   m_embeddedWebServer.reset(new http::server::cWebem(m_configAddress, m_configPort, m_configDocRoot));
   YADOMS_LOG(info) << "WebServer configure to " << m_configAddress << ":" << m_configPort;
   YADOMS_LOG(debug) << "WebServer root directory " << m_configDocRoot;

   m_embeddedWebServer->RegisterCustomCode(m_restHandler->getRestKeyword().c_str(), boost::bind(&CRestHandler::manageRestRequests,m_restHandler.get(), _1));
   m_embeddedWebServer->Run();
}

void CWebServer::stop()
{
   if(m_embeddedWebServer.get())
      m_embeddedWebServer->Stop();
}

boost::shared_ptr<IRestHandler> CWebServer::getRestHandler()
{
   return m_restHandler;
}
