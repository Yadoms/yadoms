#include "stdafx.h"
#include "RestHandler.h"
#include <shared/Log.h>

CRestHandler::CRestHandler(const std::string & restBaseKeyword)
   :m_restBaseKeyword(restBaseKeyword)
{
}

CRestHandler::~CRestHandler()
{
}

const std::string & CRestHandler::getRestKeyword()
{ 
   return m_restBaseKeyword; 
}

void CRestHandler::registerRestService(boost::shared_ptr<IRestService> restService)
{
   if(restService.get() != NULL)
      m_restService.push_back(restService);
}


void CRestHandler::initialize()
{
   BOOST_FOREACH(boost::shared_ptr<IRestService> restService, m_restService)
   {
      if(restService.get() != NULL)
         restService->configureDispatcher(m_restDispatcher);
   }
}

std::vector<std::string> CRestHandler::parseUrl(const std::string & url)
{
    std::vector<std::string> strs;
    //split on slash or anti slash
    boost::split(strs, url, boost::is_any_of("/\\"), boost::algorithm::token_compress_on);
    //remove empty strings
    //do not use std::empty in std::remove_if because MacOs Clang do not support it
    std::vector<std::string>::iterator i = strs.begin();
    while(i != strs.end())
    {
        if(i->empty())
        {
            i = strs.erase(i);
        }
        else
        {
            ++i;
        }
    }
    
    return strs;
}


std::string CRestHandler::manageRestRequests(const http::server::request & request)
{
   // Decode url to path.
   std::string request_path;
   try
   {
      std::vector<std::string> parameters;

      if (http::server::request_handler::url_decode(request.uri, request_path))
      {
         //remove the fist /rest/ string
         request_path = request_path.substr(m_restBaseKeyword.size());

         parameters = parseUrl(request_path);

         //dispatch url to rest dispatcher
         CJson js = m_restDispatcher.dispath(request.method, parameters);
         return m_jsonSerializer.serialize(js);
      }
   }
   catch(std::exception &ex)
   {
      YADOMS_LOG(error) << "An exception occured in treating REST url : " << request_path << std::endl << "Exception : " << ex.what();
   }
   catch(...)
   {
      YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << request_path;
   }
   return "";
}





