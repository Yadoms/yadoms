#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPCredentials.h>
#include <Poco/URI.h>
#include <boost/property_tree/xml_parser.hpp>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "Poco/StreamCopier.h"

namespace http
{
   void CHttpMethods::parseNode(shared::CDataContainer &container, boost::property_tree::ptree node)
   {
      boost::property_tree::ptree::const_iterator end = node.end();
      std::string attributeName;
      std::string attributeValue;

      for (boost::property_tree::ptree::const_iterator it = node.begin(); it != end; ++it)
      {
         //YADOMS_LOG(information) << "it->first : " << it->first;
         //YADOMS_LOG(information) << "it->second.data() : " << it->second.data();

         if (it->second.size() != 0)
         {
            shared::CDataContainer subNode;
            parseNode(container/*subNode*/, it->second);
            //container.set(it->first, subNode);
         }
         else
         {
            if (it->first == "id" || it->first == "var")
               attributeName = it->second.data();

            if (it->first == "value")
               attributeValue = it->second.data();
            //container.set(it->first, it->second.data());
         }
      }

      container.set(attributeName, attributeValue);
   }

   shared::CDataContainer CHttpMethods::SendGetRequest(const std::string& url)
   {
      return SendGetRequest(url, shared::CDataContainer());
   }

   bool CHttpMethods::SendGetRequest(const std::string& url,
                                     const shared::CDataContainer& parameters,
                                     boost::function1<void, shared::CDataContainer&> onReceive,
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

         Poco::Net::HTTPCredentials creds("admin", "wes");

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);
         
         Poco::Net::HTTPResponse response;
         session.sendRequest(request);

         auto& rs = session.receiveResponse(response);
         std::string buffer;
         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
         {
            creds.authenticate(request, response);
            session.sendRequest(request);
            auto& rs = session.receiveResponse(response);
            
            std::ostringstream oss;
            Poco::StreamCopier::copyStream(rs, oss);
            buffer = oss.str();
         }

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
         {
            shared::CDataContainer data;
            std::istringstream oss1(buffer);

            if (XmlResponseReader(oss1, response, data))
            {
               onReceive(data);
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
         auto message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.message()).str();
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }
   }

   shared::CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                                       const shared::CDataContainer& parameters,
                                                       const boost::posix_time::time_duration& timeout)
   {
      shared::CDataContainer responseData;

      SendGetRequest(url,
                     parameters,
                     [&](shared::CDataContainer& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   bool CHttpMethods::XmlResponseReader(std::istream& stream,
                                        Poco::Net::HTTPResponse& httpresponse,
                                        shared::CDataContainer& response)
   {
      std::string content;

      if (boost::icontains(httpresponse.getContentType(), "text/xml"))
      {
         boost::property_tree::ptree pt;
         boost::property_tree::xml_parser::read_xml(stream, pt); 
            
         parseNode(response, pt);

         response.printToLog(YADOMS_LOG(information));

         //request content may be empty
         return true;
      }

      return false;
   }
} // namespace http