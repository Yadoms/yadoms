#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/URI.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include <Poco/StreamCopier.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace http
{
   boost::posix_time::time_duration CHttpMethods::httpRequestDefaultTimeout(boost::posix_time::time_duration(boost::posix_time::seconds(45)));

   bool CHttpMethods::SendGetRequest(
      const std::string& url,
      const boost::shared_ptr<shared::CDataContainer>& credentials,
      const boost::shared_ptr<shared::CDataContainer>& parameters,
      boost::function1<void, boost::shared_ptr<shared::CDataContainer>&> onReceive,
      http::httpContext& context,
      const boost::posix_time::time_duration& timeout)
   {
      try {
         auto mapParameters = parameters->getAsMap<std::string>();
         Poco::URI uri(url);

         if (!parameters->empty()) {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first, parametersIterator.second);
         }

         Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
         session.setTimeout(Poco::Timespan(timeout.seconds(), 0));

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
            uri.getPathAndQuery(),
            Poco::Net::HTTPMessage::HTTP_1_1);

         Poco::Net::HTTPResponse response;

         if (!context.basicCredential()) {
            session.sendRequest(request);
            session.receiveResponse(response);
         }

         // Retry for protected equipements
         if ((response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED && !context.basicCredential())) {
            context.setBasicCredential(true);
         }

         std::string buffer;
         if (context.basicCredential()) 
         {
            Poco::Net::HTTPBasicCredentials creds(credentials->get<std::string>("user"), credentials->get<std::string>("password"));
            creds.authenticate(request);
            session.sendRequest(request);
            auto& receiveSecureStream = session.receiveResponse(response);

            std::ostringstream oss;
            Poco::StreamCopier::copyStream(receiveSecureStream, oss);
            buffer = oss.str();
         }

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
         {
            std::istringstream oss1(buffer);
            boost::shared_ptr<shared::CDataContainer> treeResponse = shared::CDataContainer::make();

            if (XmlResponseReader(oss1, response, treeResponse)) {
               onReceive(treeResponse);
               return true;
            }

            auto message = (boost::format("content not yet managed : %1%") % response.getContentType()).str();
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
         else if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED) {
            auto message = "HTTP : unauthorized access";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
         else
         {
         }

         auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }
      catch (Poco::Exception& e)
      {
         const std::string message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.displayText()).str();
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }
   }

   boost::shared_ptr<shared::CDataContainer> CHttpMethods::SendGetRequest(
      const std::string& url,
      const boost::shared_ptr<shared::CDataContainer>& parameters,
      http::httpContext& context,
      const boost::posix_time::time_duration& timeout)
   {
      boost::shared_ptr<shared::CDataContainer> responseData = shared::CDataContainer::make();
      boost::shared_ptr<shared::CDataContainer> credentials = shared::CDataContainer::make();

      SendGetRequest(
         url,
         credentials,
         parameters,
         [&](const boost::shared_ptr<shared::CDataContainer>& data)
         {
            responseData = data;
         },
         context,
            timeout);

      return responseData;
   }

   boost::shared_ptr<shared::CDataContainer> CHttpMethods::SendGetRequest(
      const std::string& url,
      const boost::shared_ptr<shared::CDataContainer>& credentials,
      const boost::shared_ptr<shared::CDataContainer>& parameters,
      http::httpContext& context,
      const boost::posix_time::time_duration& timeout)
   {
      boost::shared_ptr<shared::CDataContainer> responseData = shared::CDataContainer::make();

      SendGetRequest(
         url,
         credentials,
         parameters,
         [&](const boost::shared_ptr<shared::CDataContainer>& data)
         {
            responseData = data;
         },
         context,
            timeout);

      return responseData;
   }

   //keep this method out of class scope, to avoid including boot/property_tree headers
   void parseNode(boost::shared_ptr<shared::CDataContainer>& container, boost::property_tree::ptree node)
   {
      boost::property_tree::ptree::const_iterator end = node.end();
      std::string attributeName;
      std::string attributeValue;

      for (boost::property_tree::ptree::const_iterator it = node.begin(); it != end; ++it)
      {
         if (it->second.size() != 0)
         {
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

      container->set(attributeName, attributeValue);
   }

   bool CHttpMethods::XmlResponseReader(std::istream& stream,
      Poco::Net::HTTPResponse& httpresponse,
      boost::shared_ptr<shared::CDataContainer>& treeResponse)
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