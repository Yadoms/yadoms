#include "stdafx.h"
#include "WebServer.h"
#include "tools/Log.h"

CWebServer::CWebServer(	   const std::string & address, const std::string & port, const std::string & doc_root )
   :m_configAddress(address), m_configPort(port), m_configDocRoot(doc_root), m_restBaseKeyword("/rest/")
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

   m_embeddedWebServer->RegisterCustomCode(m_restBaseKeyword.c_str(), boost::bind(&CWebServer::manageRestRequests,this, _1));

   m_embeddedWebServer->Run();
}

void CWebServer::stop()
{
   m_embeddedWebServer->Stop();
}

void CWebServer::configureRestService(boost::shared_ptr<IRestService> restService)
{
   m_restServices.insert(std::make_pair(restService->getRestKeyword(), restService));
}


std::string CWebServer::manageRestRequests(const http::server::request & request)
{
   // Decode url to path.
	std::string request_path;
	if (http::server::request_handler::url_decode(request.uri, request_path))
	{
      //remove the fist /rest/ string
      request_path = request_path.substr(m_restBaseKeyword.size());

      //browse each registered rest service until finding one which match 
      std::map<std::string , boost::shared_ptr<IRestService> >::iterator pRestService;
      for(pRestService = m_restServices.begin(); pRestService != m_restServices.end(); pRestService++)
      {
         //check if rest service match
         if(boost::algorithm::starts_with(request_path, pRestService->first ))
         { 
            //parse remaining parameters (maybe empty)
            std::string parametersFromRequest  = request_path.substr(pRestService->first.size());
            std::vector<std::string> parameters = CSJonParser::ParseUrl(parametersFromRequest);

            IRestService * baseService = pRestService->second.get();
            if(request.method == "GET")
            {
               IReadRestService * pRead = dynamic_cast<IReadRestService*>(baseService);
               if(pRead != NULL)
               {
                  std::ostringstream buf; 
                  if(parameters.size() == 0)
                  {
                     CJson js = pRead->readObjects();
                     boost::property_tree::json_parser::write_json(buf, js, false);
                  }
                  else
                  {
                     CJson js = pRead->readObject(parameters[0]);
                     boost::property_tree::json_parser::write_json(buf, js, false);
                  }
                  return buf.str();
               }

            }



         }
      }
		
	}

   return "";
}