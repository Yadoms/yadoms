#include "stdafx.h"
#include "WebServer.h"
#include "tools/Log.h"

CWebServer::CWebServer(	   const std::string & address, const std::string & port, const std::string & doc_root )
   :m_configAddress(address), m_configPort(port), m_configDocRoot(doc_root)
{
}

CWebServer::~CWebServer()
{

}


// IWebServer implementation
void CWebServer::start()
{
   m_embeddedWebServer.reset(new http::server::cWebem(m_configAddress, m_configPort, m_configDocRoot));
   YADOMS_LOG(info) << "WebServer configure to " << m_configAddress << ":" << m_configPort;
   YADOMS_LOG(debug) << "WebServer root directory " << m_configDocRoot;

   m_embeddedWebServer->Run();
}

void CWebServer::stop()
{
   m_embeddedWebServer->Stop();
}

