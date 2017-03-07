#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/URI.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>


namespace shared
{
   CDataContainer CHttpMethods::SendGetRequest(const std::string& url)
   {
      return SendGetRequest(url, CDataContainer());
   }

   bool CHttpMethods::SendGetRequest(const std::string& url,
                                     const CDataContainer& parameters,
                                     boost::function1<void, CDataContainer&> onReceive,
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
         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);

         session.setTimeout(Poco::Timespan(timeout.seconds(), 0));
         session.sendRequest(request);

         Poco::Net::HTTPResponse response;

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
         {
            CDataContainer data;

            if (JsonResponseReader(session, response, data))
            {
               onReceive(data);
               return true;
            }

            auto message = (boost::format("content not yet managed : %1%") % response.getContentType()).str();
            YADOMS_LOG(error) << message;
            throw exception::CException(message);
         }

         auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(error) << message;
         throw exception::CException(message);
      }
      catch (Poco::Exception& e)
      {
         auto message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.message()).str();
         YADOMS_LOG(error) << message;
         throw exception::CException(message);
      }
   }

   CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                               const CDataContainer& parameters,
                                               const boost::posix_time::time_duration& timeout)
   {
      CDataContainer responseData;

      SendGetRequest(url,
                     parameters,
                     [&](CDataContainer& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   bool CHttpMethods::JsonResponseReader(Poco::Net::HTTPClientSession& session,
                                         Poco::Net::HTTPResponse& httpresponse,
                                         CDataContainer& response)
   {
      std::string content;
      auto& rs = session.receiveResponse(httpresponse);

      if (boost::icontains(httpresponse.getContentType(), "application/json"))
      {
         if (httpresponse.hasContentLength())
         {
            content.resize(static_cast<unsigned int>(httpresponse.getContentLength()));
            rs.read(const_cast<char*>(content.c_str()), httpresponse.getContentLength());
            response.deserialize(content);
         }

         //request content may be empty
         return true;
      }

      return false;
   }
} // namespace shared


