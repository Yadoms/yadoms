#include "stdafx.h"
#include "RestHandler.h"
#include <shared/Log.h>
#include "web/rest/json/JsonResult.h"
#include "web/rest/json/JsonCollectionSerializer.h"

namespace web { namespace webem {

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

   void CRestHandler::registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService)
   {
      if(restService.get() != NULL)
         m_restService.push_back(restService);
   }


   void CRestHandler::initialize()
   {
      BOOST_FOREACH(boost::shared_ptr<web::rest::service::IRestService> restService, m_restService)
      {
         if(restService.get() != NULL)
            restService->configureDispatcher(m_restDispatcher);
      }
      m_restDispatcher.sortRestMethod();
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

            //parse url to parameters
            parameters = parseUrl(request_path);

            //parse content to json format
            web::rest::json::CJson requestContent;
            try
            {
               BOOST_FOREACH(http::server::header headerData, request.headers)
               {
                  if(boost::iequals(headerData.name, "contentType") || boost::iequals(headerData.name, "Content-Type") )
                  {
                     if(boost::ifind_first(headerData.value, "application/json"))
                     {
                        requestContent = web::rest::json::CJsonSerializer::deserialize(request.content);                  
                     }
                     else
                     {
                        YADOMS_LOG(warning) << "Ignore content because its format is not supported";
                     }
                  }

               }

            }
            catch(std::exception &ex)
            {
               YADOMS_LOG(error) << "Fail to read request content as Json format. Exception : " << ex.what();
               return web::rest::json::CJsonSerializer::serialize(web::rest::json::CJsonResult::GenerateError("Fail to read request content as Json format"));
            }

            //dispatch url to rest dispatcher
            web::rest::json::CJson js = m_restDispatcher.dispath(request.method, parameters, requestContent);
            return web::rest::json::CJsonSerializer::serialize(js);
         }
         else
         {
            return web::rest::json::CJsonSerializer::serialize(web::rest::json::CJsonResult::GenerateError("Rest handler : cannot decode url"));
         }
      }
      catch(std::exception &ex)
      {
         YADOMS_LOG(error) << "An exception occured in treating REST url : " << request_path << std::endl << "Exception : " << ex.what();
         return web::rest::json::CJsonSerializer::serialize(web::rest::json::CJsonResult::GenerateError(ex));
      }
      catch(...)
      {
         YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << request_path;
         return web::rest::json::CJsonSerializer::serialize(web::rest::json::CJsonResult::GenerateError("An unknown exception occured in treating REST url : " + request_path));
      }
   }

} //namespace webem
} //namespace web


