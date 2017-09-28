#include "stdafx.h"
#include "HttpMethods.h"
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLException.h>
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
                                     const CDataContainer& headerParameters,
                                     const CDataContainer& parameters,
                                     boost::function1<void, CDataContainer&> onReceive,
                                     const boost::posix_time::time_duration& timeout)
   {
      try
      {
         auto mapParameters = parameters.getAsMap();
         auto mapheaderParameters = headerParameters.getAsMap();
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

         if (!headerParameters.empty())
         {
            for (const auto& headerparametersIterator : mapheaderParameters)
               request.add(headerparametersIterator.first, headerparametersIterator.second);
         }

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

   bool CHttpMethods::SendGetSecureRequest(const std::string& url,
                                           const CDataContainer& headerParameters,
                                           const CDataContainer& parameters,
                                           boost::function1<void, CDataContainer&> onReceive,
                                           const boost::posix_time::time_duration& timeout)
   {
      try
      {
         auto mapParameters = parameters.getAsMap();
         auto mapheaderParameters = headerParameters.getAsMap();
         Poco::URI uri(url);

         if (!parameters.empty())
         {
            for (const auto& parametersIterator : mapParameters)
               uri.addQueryParameter(parametersIterator.first, parametersIterator.second);
         }

         Poco::Crypto::OpenSSLInitializer::initialize();

         const Poco::Net::Context::Ptr context(new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, false, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH"));
         Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
         Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET,
                                        uri.getPathAndQuery(),
                                        Poco::Net::HTTPMessage::HTTP_1_1);

         if (!headerParameters.empty())
         {
            for (const auto& headerparametersIterator : mapheaderParameters)
               request.add(headerparametersIterator.first, headerparametersIterator.second);
         }

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
      catch (const Poco::Net::SSLException& e)
      {
         std::cerr << e.what() << ": " << e.message() << std::endl;
      }
      catch (Poco::Exception& e)
      {
         auto message = (boost::format("Fail to send get http request \"%1%\" : %2%") % url % e.message()).str();
         YADOMS_LOG(error) << message;
         throw exception::CException(message);
      }
   }

   //https://stackoverflow.com/questions/1499086/poco-c-net-ssl-how-to-post-https-request
   /*
   try
   {
   const Poco::URI uri( "https://localhost.com" );
   const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
   Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context );
   Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, "/login.php" );
   req.setContentType("application/x-www-form-urlencoded\r\n");
   req.setKeepAlive(true);

   std::string reqBody("username=???&password=???&action_login=Log+In\r\n\r\n");
   req.setContentLength( reqBody.length() );

   //Poco::Net::HTTPBasicCredentials cred("???", "???");
   //cred.authenticate(req);
   session.sendRequest(req) << reqBody;
   Poco::Net::HTTPResponse res;
   std::istream& rs = session.receiveResponse(res);
   std::string resp;

   std::vector<Poco::Net::HTTPCookie> cookies;
   res.getCookies( cookies );
   }
   catch( const Poco::Net::SSLException& e )
   {
   std::cerr << e.what() << ": " << e.message() << std::endl;
   }
   catch( const std::exception& e )
   {
   std::cerr << e.what() << std::endl;;
   }
   */

   CDataContainer CHttpMethods::SendGetRequest(const std::string& url,
                                               const CDataContainer& parameters,
                                               const boost::posix_time::time_duration& timeout)
   {
      CDataContainer responseData;

      SendGetRequest(url,
                     CDataContainer(), // no header parameters
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


