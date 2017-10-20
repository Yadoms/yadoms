#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/SSLException.h>
#include <shared/exception/Exception.hpp>
#include <shared/Log.h>
#include "SecureSession.h"
#include "StandardSession.h"

namespace shared
{
   CDataContainer CHttpMethods::SendGetRequest(const std::string& url)
   {
      return SendGetRequest(url, CDataContainer());
   }

   bool CHttpMethods::SendGetRequest(const boost::shared_ptr<IHTTPSession> session,
                                     const CDataContainer& headerParameters,
                                     const CDataContainer& parameters,
                                     boost::function1<void, CDataContainer&> onReceive,
                                     const boost::posix_time::time_duration& timeout)
   {
      try
      {
         auto mapParameters = parameters.getAsMap();
         auto mapheaderParameters = headerParameters.getAsMap();
         Poco::URI uri(session->getUrl());

         if (!parameters.empty())
         {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first, parametersIterator.second);
         }

         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);

         if (!headerParameters.empty())
         {
            for (const auto& headerparametersIterator : mapheaderParameters)
               request.add(headerparametersIterator.first, headerparametersIterator.second);
         }

         session->setTimeout(timeout);
         session->sendRequest(request);

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

            return false;
         }

         auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
         YADOMS_LOG(error) << message;
         throw exception::CException(message);

         return false;
      }
	  catch (const Poco::Net::SSLException& e)
	  {
		  std::cerr << e.what() << ": " << e.message() << std::endl;
        return false;
	  }
     catch (Poco::Exception& e)
     {
        auto message = (boost::format("Fail to send get http request \"%1%\" : %2%") % session->getUrl() % e.message()).str();
        YADOMS_LOG(error) << message;
        throw exception::CException(message);
        return false;
      }
   }

   CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                               const CDataContainer& parameters,
                                               const boost::posix_time::time_duration& timeout)
   {
      CDataContainer responseData;
	  boost::shared_ptr<StandardSession> session=boost::make_shared<StandardSession>(url);

      SendGetRequest(session,
                     CDataContainer(), // no header parameters
                     parameters,
                     [&](CDataContainer& data)
                     {
                        responseData = data;
                     },
                     timeout);

      return responseData;
   }

   bool CHttpMethods::JsonResponseReader(const boost::shared_ptr<IHTTPSession> session,
                                         Poco::Net::HTTPResponse& httpresponse,
                                         CDataContainer& response)
   {
      std::string content;
      auto& rs = session->receiveResponse(httpresponse);

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