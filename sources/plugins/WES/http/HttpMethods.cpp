#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPCredentials.h>
#include <Poco/URI.h>
#include <boost/property_tree/xml_parser.hpp>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "Poco/StreamCopier.h"
#include "timeOutException.hpp"

namespace http
{
   bool CHttpMethods::SendGetRequest(const std::string& url,
                                     const shared::CDataContainer& credentials,
                                     const shared::CDataContainer& parameters,
                                     boost::function1<void, boost::property_tree::ptree&> onReceive,
                                     const boost::posix_time::time_duration& timeout)
   {
      try
      {
         auto mapParameters = parameters.getAsMap();
         Poco::URI uri(url);

         if (!parameters.empty())
         {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first, parametersIterator.second);
         }

         Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
         session.setTimeout(Poco::Timespan(timeout.seconds(), 0));

         Poco::Net::HTTPCredentials creds(credentials.get<std::string>("user"), 
                                          credentials.get<std::string>("password"));

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);
         
         Poco::Net::HTTPResponse response;
         session.sendRequest(request);
         session.receiveResponse(response);

         std::string buffer;
         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
         {
            creds.authenticate(request, response);
            session.sendRequest(request);
            auto& receiveSecureStream = session.receiveResponse(response);
            
            std::ostringstream oss;
            Poco::StreamCopier::copyStream(receiveSecureStream, oss);
            buffer = oss.str();
         }

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
         {
            shared::CDataContainer data;
            std::istringstream oss1(buffer);
            boost::property_tree::ptree treeResponse;

            if (XmlResponseReader(oss1, response, treeResponse))
            {
               onReceive(treeResponse);
               return true;
            }

            if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
            {
               auto message = "HTTP : unauthorized access";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            auto message = (boost::format("content not yet managed : %1%") % response.getContentType()).str();
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }

         auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }
      catch (Poco::Exception& e)
      {
         std::string message;

         // Sometimes message return nothing. In this case, we return what()
         if (e.message().size() != 0)
            message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.message()).str();
         else
            message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.what()).str();

         if (boost::contains(e.what(),"Timeout"))
            throw CTimeOutException(message);
         else
            throw shared::exception::CException(message);
      }
   }

   boost::property_tree::ptree CHttpMethods::SendGetRequest(const std::string& url,
                                                            const shared::CDataContainer& parameters,
                                                            const boost::posix_time::time_duration& timeout)
   {
      boost::property_tree::ptree responseData;
      shared::CDataContainer credentials;

      SendGetRequest(url,
                     credentials,
                     parameters,
                     [&](boost::property_tree::ptree& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   boost::property_tree::ptree CHttpMethods::SendGetRequest(const std::string& url,
                                                            const shared::CDataContainer& credentials,
                                                            const shared::CDataContainer& parameters,
                                                            const boost::posix_time::time_duration& timeout)
   {
      boost::property_tree::ptree responseData;

      SendGetRequest(url,
                     credentials,
                     parameters,
                     [&](boost::property_tree::ptree& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   bool CHttpMethods::XmlResponseReader(std::istream& stream,
                                        Poco::Net::HTTPResponse& httpresponse,
                                        boost::property_tree::ptree& treeResponse)
   {
      std::string content;

      if (boost::icontains(httpresponse.getContentType(), "text/xml"))
      {
         boost::property_tree::xml_parser::read_xml(stream, treeResponse);

         //request content may be empty
         return true;
      }

      return false;
   }
} // namespace http