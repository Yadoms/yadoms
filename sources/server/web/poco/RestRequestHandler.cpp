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
         std::vector< boost::shared_ptr<rest::service::IRestService> >::const_iterator i;
         for (i = services.begin(); i != services.end(); ++i)
            registerRestService(*i);
         initialize();
      }

      CRestRequestHandler::~CRestRequestHandler()
      {
      }

      const std::string& CRestRequestHandler::getRestKeyword() const
      {
         return m_restBaseKeyword;
      }

      std::vector<std::string> CRestRequestHandler::parseUrl(const std::string& url)
      {
         std::vector<std::string> strs;
         std::vector<std::string> results;
         //split on slash or anti slash
         boost::split(strs, url, boost::is_any_of("/\\"), boost::algorithm::token_compress_on);
         //remove empty strings
         //do not use std::empty in std::remove_if because MacOs Clang do not support it
         auto i = strs.begin();
         while (i != strs.end())
         {
            if (i->empty())
            {
               i = strs.erase(i);
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
               content.resize(static_cast<unsigned int>(request.getContentLength()));
               request.stream().read(const_cast<char*>(content.c_str()), request.getContentLength());
            }

            //dispatch url to rest dispatcher
            auto js = m_restDispatcher.dispath(request.getMethod(), parameters, content);

            boost::posix_time::ptime start, stop;
            start = boost::posix_time::microsec_clock::local_time();
            std::string temp = js->serialize();

            stop = boost::posix_time::microsec_clock::local_time();

            boost::posix_time::time_duration dur = stop - start;
            long milliseconds = dur.total_milliseconds();

            boost::format output("%.2f");
            output % (milliseconds);
            YADOMS_LOG(information) << "time 3: " << output << std::endl;
            //YADOMS_LOG(information) << temp;
            return temp;
         }

         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "An exception occured in treating REST url : " << request_path << std::endl << "Exception : " << ex.what();
            return web::rest::CResult::GenerateError(ex)->serialize();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "An unknown exception occured in treating REST url : " << request_path;
            return web::rest::CResult::GenerateError("An unknown exception occured in treating REST url : " + request_path)->serialize();
         }
      }

      void CRestRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request,
                                              Poco::Net::HTTPServerResponse& response)
      {
         YADOMS_LOG(trace) << "Rest request : [" << request.getMethod() << "] : " << request.getURI();

         std::string answer = manageRestRequests(request);
         response.setContentType("application/json");
         std::ostream& ostr = response.send();
         ostr << answer;
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


