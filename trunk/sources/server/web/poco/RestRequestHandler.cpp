#include "stdafx.h"
#include "RestRequestHandler.h"
#include <shared/Log.h>
#include "web/rest/Result.h"
#include <Poco/Net/HTTPBasicCredentials.h>
#include <shared/Log.h>
#include <shared/tools/Debug.h>

namespace web {
   namespace poco {

      CRestRequestHandler::CRestRequestHandler(const std::string & restBaseKeyword, const std::vector< boost::shared_ptr<rest::service::IRestService> > & services)
         :m_restBaseKeyword(restBaseKeyword)
      {
         std::vector< boost::shared_ptr<rest::service::IRestService> >::const_iterator i;
         for (i = services.begin(); i != services.end(); ++i)
            registerRestService(*i);
         initialize();
      }

      CRestRequestHandler::~CRestRequestHandler()
      {
      }

      const std::string & CRestRequestHandler::getRestKeyword()
      {
         return m_restBaseKeyword;
      }


      std::vector<std::string> CRestRequestHandler::parseUrl(const std::string & url)
      {
         std::vector<std::string> strs;
         //split on slash or anti slash
         boost::split(strs, url, boost::is_any_of("/\\"), boost::algorithm::token_compress_on);
         //remove empty strings
         //do not use std::empty in std::remove_if because MacOs Clang do not support it
         std::vector<std::string>::iterator i = strs.begin();
         while (i != strs.end())
         {
            if (i->empty())
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

      std::string CRestRequestHandler::manageRestRequests(Poco::Net::HTTPServerRequest& request)
      {
         // Decode url to path.
         std::string request_path = request.getURI();

         try
         {
            std::vector<std::string> parameters;

            //remove the fist /rest/ string
            request_path = request_path.substr(m_restBaseKeyword.size());

            //parse url to parameters
            parameters = parseUrl(request_path);

            std::string content;
            if (request.hasContentLength())
            {
               content.resize((unsigned int)request.getContentLength());
               request.stream().read((char*)content.c_str(), request.getContentLength());
            }

            //dispatch url to rest dispatcher
            shared::CDataContainer js = m_restDispatcher.dispath(request.getMethod(), parameters, content);
            return js.serialize();
         }

         catch (std::exception &ex)
         {
            YADOMS_LOG(error) << "An exception occured in treating REST url : " << request_path << std::endl << "Exception : " << ex.what();
            return web::rest::CResult::GenerateError(ex).serialize();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << request_path;
            return web::rest::CResult::GenerateError("An unknown exception occured in treating REST url : " + request_path).serialize();
         }
      }

      void CRestRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
      {
         //log thread ID
         shared::tools::CDebug::LogThreadId("RestRequestHandler");

         std::string answer = manageRestRequests(request);
         response.setContentType("application/json");
         std::ostream& ostr = response.send();
         ostr << answer;
      }

      void CRestRequestHandler::registerRestService(boost::shared_ptr<web::rest::service::IRestService> restService)
      {
         if (restService.get() != NULL)
            m_restService.push_back(restService);
      }


      void CRestRequestHandler::initialize()
      {
         for (std::vector<boost::shared_ptr<web::rest::service::IRestService> >::iterator i = m_restService.begin(); i != m_restService.end(); ++i)
         {
            if (i->get() != NULL)
               (*i)->configureDispatcher(m_restDispatcher);
         }
      }


   } //namespace poco
} //namespace web

