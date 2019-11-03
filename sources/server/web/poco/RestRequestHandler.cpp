#include "stdafx.h"
#include "RestRequestHandler.h"
#include <shared/Log.h>
#include "web/rest/Result.h"
#include <Poco/URI.h>

namespace web
{
   namespace poco
   {
      CRestRequestHandler::CRestRequestHandler(const std::string& restBaseKeyword,
                                               const std::vector< boost::shared_ptr<rest::service::IRestService> >& services)
         : m_restBaseKeyword(restBaseKeyword)
      {
         for (auto i = services.begin(); i != services.end(); ++i)
            CRestRequestHandler::registerRestService(*i);
         CRestRequestHandler::initialize();
      }

      const std::string& CRestRequestHandler::getRestKeyword() const
      {
         return m_restBaseKeyword;
      }

      std::vector<std::string> CRestRequestHandler::parseUrl(const std::string& url)
      {
         std::vector<std::string> strings;
         std::vector<std::string> results;
         //split on slash or anti slash
         boost::split(strings, url, boost::is_any_of("/\\"), boost::algorithm::token_compress_on);
         //remove empty strings
         //do not use std::empty in std::remove_if because MacOs Clang do not support it
         auto i = strings.begin();
         while (i != strings.end())
         {
            if (i->empty())
            {
               i = strings.erase(i);
            }
            else
            {
               //each url parameter must be url decoded
               std::string decodedString;
               Poco::URI::decode(*i, decodedString);
               results.push_back(decodedString);

               ++i;
            }
         }

         return results;
      }

      std::string CRestRequestHandler::manageRestRequests(Poco::Net::HTTPServerRequest& request)
      {
         // Decode url to path.
         auto requestPath = request.getURI();

         try
         {
            //remove the fist /rest/ string
            requestPath = requestPath.substr(m_restBaseKeyword.size());

            //parse url to parameters
            const auto parameters = parseUrl(requestPath);

            std::string content;
            if (request.hasContentLength())
            {
               content.resize(static_cast<unsigned int>(request.getContentLength()));
               request.stream().read(const_cast<char*>(content.c_str()), request.getContentLength());
            }

            //dispatch url to rest dispatcher
            const auto js = m_restDispatcher.dispatch(request.getMethod(), parameters, content);
            auto temp = js->serialize();
            return temp;
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "An exception occured in treating REST url : " << requestPath << std::endl << "Exception : " << ex.what();
            return web::rest::CResult::GenerateError(ex)->serialize();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << requestPath;
            return web::rest::CResult::GenerateError("An unknown exception occured in treating REST url : " + requestPath)->serialize();
         }
      }

      void CRestRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                              Poco::Net::HTTPServerResponse& response)
      {
         YADOMS_LOG(trace) << "Rest request : [" << request.getMethod() << "] : " << request.getURI();

         const auto answer = manageRestRequests(request);
         response.setContentType("application/json");
         auto& stream = response.send();
         stream << answer;
      }

      void CRestRequestHandler::registerRestService(boost::shared_ptr<rest::service::IRestService> restService)
      {
         if (restService.get() != nullptr)
            m_restService.push_back(restService);
      }

      void CRestRequestHandler::initialize()
      {
         for (auto i = m_restService.begin(); i != m_restService.end(); ++i)
         {
            if (i->get() != nullptr)
               (*i)->configureDispatcher(m_restDispatcher);
         }
      }
   } //namespace poco
} //namespace web


