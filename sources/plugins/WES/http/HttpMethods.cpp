#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPCredentials.h>
#include <Poco/URI.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include <Poco/StreamCopier.h>
#include "timeOutException.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace http
{
   boost::posix_time::time_duration CHttpMethods::httpRequestDefaultTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(45)));

   bool CHttpMethods::SendGetRequest(const std::string& url,
                                     const shared::CDataContainer& credentials,
                                     const shared::CDataContainer& parameters,
                                     boost::function1<void, shared::CDataContainer&> onReceive,
                                     const boost::posix_time::time_duration& timeout)
   {
      try
      {
         auto mapParameters = parameters.getAsMap<std::string>();
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

         // Retry for protected equipements
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
            shared::CDataContainer treeResponse;

            if (XmlResponseReader(oss1, response, treeResponse))
            {
               onReceive(treeResponse);
               return true;
            }

            auto message = (boost::format("content not yet managed : %1%") % response.getContentType()).str();
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
         else if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
         {
            auto message = "HTTP : unauthorized access";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
         else
         { }

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

   shared::CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                                            const shared::CDataContainer& parameters,
                                                            const boost::posix_time::time_duration& timeout)
   {
      shared::CDataContainer responseData;
      shared::CDataContainer credentials;

      SendGetRequest(url,
                     credentials,
                     parameters,
                     [&](shared::CDataContainer& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   shared::CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                                            const shared::CDataContainer& credentials,
                                                            const shared::CDataContainer& parameters,
                                                            const boost::posix_time::time_duration& timeout)
   {
      shared::CDataContainer responseData;

      SendGetRequest(url,
                     credentials,
                     parameters,
                     [&](shared::CDataContainer& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   //keep this method out of class scope, to avoid including boot/property_tree headers
   void parseNode(shared::CDataContainer &container, boost::property_tree::ptree node)
   {
      boost::property_tree::ptree::const_iterator end = node.end();
      std::string attributeName;
      std::string attributeValue;

      for (boost::property_tree::ptree::const_iterator it = node.begin(); it != end; ++it)
      {
         if (it->second.size() != 0)
         {
            shared::CDataContainer subNode;
            parseNode(container, it->second);
         }
         else
         {
            if (it->first == "id" || it->first == "var")
               attributeName = it->second.data();

            if (it->first == "value")
               attributeValue = it->second.data();
         }
      }

      container.set(attributeName, attributeValue);
   }

   bool CHttpMethods::XmlResponseReader(std::istream& stream,
                                        Poco::Net::HTTPResponse& httpresponse,
                                        shared::CDataContainer& treeResponse)
   {
      if (boost::icontains(httpresponse.getContentType(), "text/xml"))
      {
         //use boostptree
         boost::property_tree::ptree local;
         boost::property_tree::xml_parser::read_xml(stream, local);
         parseNode(treeResponse, local);

         //request content may be empty
         return true;
      }

      return false;
   }
} // namespace http