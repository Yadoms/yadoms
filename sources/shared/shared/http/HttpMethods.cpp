#include "stdafx.h"
#include "HttpMethods.h"
#include "HttpException.hpp"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/SSLException.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "SecureSession.h"
#include "StandardSession.h"
#include <Poco/URI.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace shared
{
   CDataContainer CHttpMethods::sendGetRequest(const std::string& url,
                                               const CDataContainer& headerParameters,
                                               const CDataContainer& parameters,
                                               const ESessionType& sessionType,
                                               const boost::posix_time::time_duration& timeout)
   {
      try
      {
         const auto& mapParameters = parameters.getAsMap<std::string>();
         Poco::URI uri(url);

         if (!parameters.empty())
         {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first,
                                     parametersIterator.second);
         }

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);

         const auto& mapHeaderParameters = headerParameters.getAsMap<std::string>();
         if (!headerParameters.empty())
         {
            for (const auto& headerParametersIterator : mapHeaderParameters)
               request.add(headerParametersIterator.first,
                           headerParametersIterator.second);
         }

         auto session(createSession(sessionType,
                                    url,
                                    timeout));

         session->sendRequest(request);

         Poco::Net::HTTPResponse response;
         auto& receivedStream = session->receiveResponse(response);

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
            return processResponse(response, receivedStream);

         const auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(warning) << message;
         throw exception::CException(message);
      }
      catch (const Poco::Net::SSLException& e)
      {
         std::cerr << e.what() << ": " << e.message() << std::endl;
         throw CHttpException(e.message());
      }
      catch (Poco::Exception& e)
      {
         const auto message = (boost::format("Fail to send GET http request \"%1%\" : %2%") % url % e.
            message()).str();
         YADOMS_LOG(warning) << message;
         YADOMS_LOG(warning) << "Request was : " << url;
         throw CHttpException(message);
      }
      catch (std::exception& e)
      {
         const auto message = (boost::format("Fail to send GET http request or interpret answer \"%1%\" : %2%") % url %
            e.what()).str();
         YADOMS_LOG(warning) << message;
         throw CHttpException(message);
      }
   }

   CDataContainer CHttpMethods::sendPostRequest(const std::string& url,
                                                const CDataContainer& headerParameters,
                                                const CDataContainer& parameters,
                                                const std::string& body,
                                                const ESessionType& sessionType,
                                                const boost::posix_time::time_duration& timeout)
   {
      try
      {
         const auto& mapParameters = parameters.getAsMap<std::string>();
         Poco::URI uri(url);

         if (!parameters.empty())
         {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first,
                                     parametersIterator.second);
         }

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);

         const auto& mapHeaderParameters = headerParameters.getAsMap<std::string>();
         if (!headerParameters.empty())
         {
            for (const auto& headerParametersIterator : mapHeaderParameters)
               request.add(headerParametersIterator.first,
                           headerParametersIterator.second);
         }

         request.setContentLength(body.length());

         auto session(createSession(sessionType,
                                    url,
                                    timeout));

         std::ostream& os = session->sendRequest(request);

         os << body;

         Poco::Net::HTTPResponse response;
         auto& receivedStream = session->receiveResponse(response);

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK || response.getStatus() == Poco::Net::HTTPResponse::HTTP_CREATED)
            return processResponse(response, receivedStream);

         const auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(warning) << message;
         throw exception::CException(message);
      }
      catch (const Poco::Net::SSLException& e)
      {
         std::cerr << e.what() << ": " << e.message() << std::endl;
         throw CHttpException(e.message());
      }
      catch (Poco::Exception& e)
      {
         const auto message = (boost::format("Fail to send Post http request \"%1%\" : %2%") % url % e.
            message()).str();
         YADOMS_LOG(warning) << message;
         YADOMS_LOG(warning) << "Request was : " << url;
         throw CHttpException(message);
      }
      catch (std::exception& e)
      {
         const auto message = (boost::format("Fail to send Post http request or interpret answer \"%1%\" : %2%") % url %
            e.what()).str();
         YADOMS_LOG(warning) << message;
         throw CHttpException(message);
      }
   }

   boost::shared_ptr<IHttpSession> CHttpMethods::createSession(const ESessionType& sessionType,
                                                               const std::string& url,
                                                               const boost::posix_time::time_duration& timeout)
   {
      boost::shared_ptr<IHttpSession> session;

      switch (sessionType)
      {
      case kStandard:
         session = boost::make_shared<CStandardSession>(url);
         break;
      case kSecured:
         session = boost::make_shared<CSecureSession>(url);
         break;
      default:
         YADOMS_LOG(error) << "HTTP session type " << sessionType << " not supported";
         throw std::invalid_argument("HTTP session type not supported");
      }

      session->setTimeout(timeout);

      return session;
   }

   CDataContainer CHttpMethods::processResponse(Poco::Net::HTTPResponse& response,
                                                std::istream& receivedStream)
   {
      if (boost::icontains(response.getContentType(), "application/json") ||
         boost::icontains(response.getContentType(), "text/json"))
      {
         return processJsonResponse(response,
                                    receivedStream);
      }
      if (boost::icontains(response.getContentType(), "application/xml") ||
         boost::icontains(response.getContentType(), "text/xml"))
      {
         return processXmlResponse(response,
                                   receivedStream);
      }
      if (boost::icontains(response.getContentType(), "text/"))
      {
         return processTextResponse(response,
                                    receivedStream);
      }
      return processRawResponse(response,
                                receivedStream);
   }

   CDataContainer CHttpMethods::processJsonResponse(Poco::Net::HTTPResponse& response,
                                                    std::istream& receivedStream)
   {
      // Content-Length is not always fulfilled so we don't use hasContentLength and getContentLength
      static const std::istreambuf_iterator<char> Eos;
      return CDataContainer(std::string(std::istreambuf_iterator<char>(receivedStream), Eos));
   }

   CDataContainer CHttpMethods::processXmlNode(const boost::property_tree::ptree& node)
   {
      CDataContainer data;
      for (const auto& it : node)
      {
         if (it.second.size() != 0)
         {
            data.set(it.first,
                     processXmlNode(it.second));
         }
         else
         {
            //TODO
            //if (it->first == "id" || it->first == "var")
            //   attributeName = it->second.data();

            //if (it->first == "value")
            //   attributeValue = it->second.data();

            //data.set(attributeName, attributeValue);
         }
      }
      
      return CDataContainer();//TODO virer
   }

   CDataContainer CHttpMethods::processXmlResponse(Poco::Net::HTTPResponse& response,
                                                   std::istream& receivedStream)
   {
      boost::property_tree::ptree tree;
      read_xml(receivedStream, tree);
      return processXmlNode(tree);
   }

   CDataContainer CHttpMethods::processTextResponse(Poco::Net::HTTPResponse& response, std::istream& receivedStream)
   {
      //TODO
      return CDataContainer();//TODO virer
   }

   CDataContainer CHttpMethods::processRawResponse(Poco::Net::HTTPResponse& response, std::istream& receivedStream)
   {
      //TODO
      return CDataContainer();//TODO virer
   }
} // namespace shared
