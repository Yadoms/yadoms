#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPSession.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <shared/Log.h>

#include "failedSendingException.hpp"
#include "invalidHTTPResultException.hpp"

namespace http
{
   shared::CDataContainerSharedPtr CHttpMethods::SendGetRequest(const std::string & url, shared::CDataContainerSharedPtr & parameters)
   {
      try
      {
         std::map<std::string, std::string> mapParameters = parameters->getAsMap<std::string>();
         Poco::URI uri(url);

         if (!parameters->empty())
         {
            for (std::map<std::string, std::string>::iterator parametersIterator = mapParameters.begin(); parametersIterator != mapParameters.end(); ++parametersIterator)
               uri.addQueryParameter(parametersIterator->first, parametersIterator->second);
         }

         Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());
         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, uri.getPathAndQuery(), Poco::Net::HTTPMessage::HTTP_1_1);

         // The time out is drastically lowered. The module is considered in the same network.
		   session.setTimeout(Poco::Timespan(2, 0));
         session.sendRequest(request);

         Poco::Net::HTTPResponse response;
         auto& rs = session.receiveResponse(response);

         if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
         {
            std::ostringstream oss;
            Poco::StreamCopier::copyStream(rs, oss);

/*       The structure of this return when a command is properly executed is not JSON compliant
            {
               "product": "IPX800_V4",
               "Success"
            }

         This part add a value to this parameter, to be JSON compliant
*/       

            std::string buff = oss.str();
            size_t successFind = buff.find("Success");

            if (successFind == std::string::npos)
               return new_CDataContainerSharedPtrP(oss.str());
            else
            {
               buff.insert(successFind + 8, ": \"0\"");
               return new_CDataContainerSharedPtrP(buff);
            }
//----------------------------------------------------------------
         }
         else
         {
            auto message = (boost::format("Invalid HTTP result : %1%") % response.getReason()).str();
            YADOMS_LOG(error) << message ;
            throw CInvalidHTTPResultException(message);
         }
      }
      catch (Poco::Exception& e) 
      {
         auto message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.message()).str();
         YADOMS_LOG(error) << message ;
         throw CFailedSendingException(message);
      }
   }
} // namespace http