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
            return jsonResponseReader(response, receivedStream);

         const auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(error) << message;
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
         YADOMS_LOG(error) << message;
         YADOMS_LOG(error) << "Request was : " << url;
         throw CHttpException(message);
      }
      catch (std::exception& e)
      {
         const auto message = (boost::format("Fail to send GET http request or interpret answer \"%1%\" : %2%") % url %
            e.what()).str();
         YADOMS_LOG(error) << message;
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

   CDataContainer CHttpMethods::jsonResponseReader(Poco::Net::HTTPResponse& response,
                                                   std::istream& receivedStream)
   {
      if (!boost::icontains(response.getContentType(), "application/json"))
      {
         YADOMS_LOG(error) << "HTTP response is not JSON";
         throw std::runtime_error("HTTP response is not JSON");
      }

      // Content-Length is not always fulfilled so we don't use hasContentLength and getContentLength
      static const std::istreambuf_iterator<char> Eos;
      return CDataContainer(std::string(std::istreambuf_iterator<char>(receivedStream), Eos));
   }
} // namespace shared
