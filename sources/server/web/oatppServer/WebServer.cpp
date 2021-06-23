#include "stdafx.h"
#include "WebServer.h"
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <utility>
#include <shared/Log.h>

#include "web/rest/ResultV2.h"

namespace web
{
   namespace oatppServer
   {
      CWebServer::CWebServer(const std::string& address,
                             bool useSsl,
                             unsigned short port,
                             unsigned short securedPort,
                             const std::string& docRoot,
                             const std::string& restKeywordBase,
                             const boost::shared_ptr<std::vector<boost::shared_ptr<rest::service::IRestService>>>& restServices,
                             const std::string& webSocketKeywordBase,
                             bool allowExternalAccess)
         : m_httpRequestHandlerFactory(std::make_shared<CHttpRequestHandlerFactory>()),
           m_restServices(restServices)
      {
         oatpp::base::Environment::init();

         // Create Router component
         const auto httpRouter = oatpp::web::server::HttpRouter::createShared();
         refreshWebRoutes(httpRouter,
                          docRoot);
         refreshRestRoutes(httpRouter,
                           restKeywordBase);

         m_httpConnectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(httpRouter);

         m_tcpConnectionProvider = oatpp::network::tcp::server::ConnectionProvider::createShared(
            {
               (address.empty() ? "0.0.0.0" : address).c_str(),
               port,
               oatpp::network::Address::IP_4
            });

         m_server = std::make_shared<oatpp::network::Server>(m_tcpConnectionProvider,
                                                             m_httpConnectionHandler);

         // Configure the factory
         m_httpRequestHandlerFactory->websiteHandlerConfigure(docRoot);
         m_httpRequestHandlerFactory->restHandlerConfigure(restKeywordBase);
         m_httpRequestHandlerFactory->webSocketConfigure(webSocketKeywordBase);
         m_httpRequestHandlerFactory->allowExternalAccess(allowExternalAccess);

         ////setup HTTPServer Params (define name and version; to match common http server configuration)
         //Poco::Net::HTTPServerParams::Ptr serverParams(new Poco::Net::HTTPServerParams());
         //serverParams->setServerName("Yadoms");
         //const auto runningInformation(shared::CServiceLocator::instance().get<IRunningInformation>());
         //serverParams->setSoftwareVersion(runningInformation->getSoftwareVersion().getVersion().toString());
         //serverParams->setKeepAlive(false); //this line fix global catch exception on multiple browser refresh

         ////in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         //Poco::Net::IPAddress ipAddress;
         //if (address == "0.0.0.0" || address.empty())
         //{
         //   ipAddress = Poco::Net::IPAddress::wildcard();
         //}
         //else
         //{
         //   if (!Poco::Net::IPAddress::tryParse(address, ipAddress))
         //   {
         //      //invalid IP provided
         //      YADOMS_LOG(warning) << "*******************************************************";
         //      YADOMS_LOG(warning) << "Invalid IP address provided in web server configuration";
         //      YADOMS_LOG(warning) << "Listening on any ip address";
         //      YADOMS_LOG(warning) << "*******************************************************";
         //      ipAddress = Poco::Net::IPAddress::wildcard();
         //   }
         //}

         //// set-up a HTTPServer instance
         ////in case of "0.0.0.0" or empty , then do not use it, just use port, listen on all interfaces
         //Poco::Net::SocketAddress sa(ipAddress, port);
         //Poco::Net::ServerSocket svs(sa);
         //if (useSSL)
         //{
         //   auto fail = false;

         //   try
         //   {
         //      const Poco::Net::SocketAddress saSecure(address, securedPort);
         //      svs = Poco::Net::SecureServerSocket(saSecure);
         //   }
         //   catch (Poco::Exception& ex)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.message();
         //      fail = true;
         //   }
         //   catch (std::exception& ex)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS: " << ex.what();
         //      fail = true;
         //   }
         //   catch (...)
         //   {
         //      //fail to download package
         //      YADOMS_LOG(error) << "Fail to configure HTTPS";
         //      fail = true;
         //   }
         //   if (fail)
         //   {
         //      //we activate the default classic web server
         //      YADOMS_LOG(warning) << "**********************************************************";
         //      YADOMS_LOG(warning) << "Using HTTP instead of HTTPS due to malformed configuration";
         //      YADOMS_LOG(warning) << "**********************************************************";
         //      const Poco::Net::SocketAddress saHttp(address, port);
         //      svs = Poco::Net::ServerSocket(saHttp);
         //   }
         //}

         //auto a = false;
         //auto b = 0;
         //svs.getLinger(a, b);
         //svs.setLinger(true, 2);
         //m_embeddedWebServer = boost::make_shared<Poco::Net::HTTPServer>(m_httpRequestHandlerFactory, svs, serverParams);
      }

      CWebServer::~CWebServer()
      {
         CWebServer::stop();
         oatpp::base::Environment::destroy();
      }


      // IWebServer implementation
      void CWebServer::start()
      {
         m_serverThread = std::thread([this]
         {
            m_server->run();
         });
      }

      void CWebServer::stop()
      {
         // First, stop the ServerConnectionProvider so we don't accept any new connections
         m_tcpConnectionProvider->stop();

         // Stop server if still running
         if (m_server->getStatus() == oatpp::network::Server::STATUS_RUNNING)
            m_server->stop();

         // Finally, stop the ConnectionHandler and wait until all running connections are closed
         m_httpConnectionHandler->stop();

         // Wait for thread stopped
         if (m_serverThread.joinable())
            m_serverThread.join();
      }

      IWebServerConfigurator* CWebServer::getConfigurator()
      {
         return m_httpRequestHandlerFactory.get();
      }

      void CWebServer::refreshWebRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                        const std::string& docRoot)
      {
         const auto pagesFiles = std::make_shared<CHttpPages>("Yadoms client", docRoot);
         httpRouter->route("GET", "/", pagesFiles);

         routeAllFiles(docRoot, httpRouter, pagesFiles);
      }

      void CWebServer::routeAllFiles(const boost::filesystem::path& rootFolder,
                                     const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                     const std::shared_ptr<CHttpPages>& pagesFiles)
      {
         for (boost::filesystem::recursive_directory_iterator end, dir(rootFolder); dir != end; ++dir)
         {
            if (dir->status().type() == boost::filesystem::directory_file)
               continue;

            //TODO faire mieux en utilisant les paths
            auto file = dir->path().string();
            auto page = file.substr(rootFolder.size());
#if _WIN32
            std::replace(page.begin(), page.end(), '\\', '/');
#endif
            httpRouter->route("GET", page.c_str(), pagesFiles);
         }
      }

      class COatppRestHandler final : public oatpp::web::server::HttpRequestHandler //TODO déplacer
      {
      public:
         explicit COatppRestHandler(std::function<boost::shared_ptr<rest::IRestAnswer>(std::map<std::string, std::string>, std::string)> handler)
            : m_handler(std::move(handler))
         {
         }

         ~COatppRestHandler() override = default;

         // oatpp::web::server::HttpRequestHandler Implementation
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
         {
            try
            {
               const auto response = m_handler(toMap(request->getPathVariables()),
                                               request->getStartingLine().method != "GET" ? request->readBodyToString()->c_str() : std::string());
               return ResponseFactory::createResponse(toStatusCode(response->code()),
                                                      oatpp::String(response->body().c_str()));
            }
            catch (const std::exception& exception)
            {
               return ResponseFactory::createResponse(Status::CODE_500,
                                                      exception.what());
            }
         }

         // [END] oatpp::web::server::HttpRequestHandler Implementation

      private:
         static std::map<std::string, std::string> toMap(const oatpp::web::url::mapping::Pattern::MatchMap& in)
         {
            std::map<std::string, std::string> out;
            for (const auto& variable : in.getVariables())
               out.emplace(variable.first.std_str(), variable.second.std_str());
            return out;
         }

         static const oatpp::web::protocol::http::Status& toStatusCode(const shared::http::ECodes& error)
         {
            switch (error)
            {
            case shared::http::ECodes::kContinueValue: return oatpp::web::protocol::http::Status::CODE_100;
            case shared::http::ECodes::kSwitchingValue: return oatpp::web::protocol::http::Status::CODE_101;
            case shared::http::ECodes::kProcessingValue: return oatpp::web::protocol::http::Status::CODE_102;
            case shared::http::ECodes::kOKValue: return oatpp::web::protocol::http::Status::CODE_200;
            case shared::http::ECodes::kCreatedValue: return oatpp::web::protocol::http::Status::CODE_201;
            case shared::http::ECodes::kAcceptedValue: return oatpp::web::protocol::http::Status::CODE_202;
            case shared::http::ECodes::kNonAuthoritativeInformationValue: return oatpp::web::protocol::http::Status::CODE_203;
            case shared::http::ECodes::kNoContentValue: return oatpp::web::protocol::http::Status::CODE_204;
            case shared::http::ECodes::kResetContentValue: return oatpp::web::protocol::http::Status::CODE_205;
            case shared::http::ECodes::kPartialContentValue: return oatpp::web::protocol::http::Status::CODE_206;
            case shared::http::ECodes::kMultiStatusValue: return oatpp::web::protocol::http::Status::CODE_207;
            case shared::http::ECodes::kIMUsedValue: return oatpp::web::protocol::http::Status::CODE_226;
            case shared::http::ECodes::kMultipleChoicesValue: return oatpp::web::protocol::http::Status::CODE_300;
            case shared::http::ECodes::kMovedPermanentlyValue: return oatpp::web::protocol::http::Status::CODE_301;
            case shared::http::ECodes::kFoundValue: return oatpp::web::protocol::http::Status::CODE_302;
            case shared::http::ECodes::kSeeOtherValue: return oatpp::web::protocol::http::Status::CODE_303;
            case shared::http::ECodes::kNotModifiedValue: return oatpp::web::protocol::http::Status::CODE_304;
            case shared::http::ECodes::kseProxyValue: return oatpp::web::protocol::http::Status::CODE_305;
            case shared::http::ECodes::kSwitchProxyValue: return oatpp::web::protocol::http::Status::CODE_306;
            case shared::http::ECodes::kTemporaryRedirectValue: return oatpp::web::protocol::http::Status::CODE_307;
            case shared::http::ECodes::kBadRequestValue: return oatpp::web::protocol::http::Status::CODE_400;
            case shared::http::ECodes::kUnauthorizedValue: return oatpp::web::protocol::http::Status::CODE_401;
            case shared::http::ECodes::kPaymentRequiredValue: return oatpp::web::protocol::http::Status::CODE_402;
            case shared::http::ECodes::kForbiddenValue: return oatpp::web::protocol::http::Status::CODE_403;
            case shared::http::ECodes::kNotFoundValue: return oatpp::web::protocol::http::Status::CODE_404;
            case shared::http::ECodes::kMethodNotAllowedValue: return oatpp::web::protocol::http::Status::CODE_405;
            case shared::http::ECodes::kNotAcceptableValue: return oatpp::web::protocol::http::Status::CODE_406;
            case shared::http::ECodes::kProxyAuthenticationRequiredValue: return oatpp::web::protocol::http::Status::CODE_407;
            case shared::http::ECodes::kRequestTimeoutValue: return oatpp::web::protocol::http::Status::CODE_408;
            case shared::http::ECodes::kConflictValue: return oatpp::web::protocol::http::Status::CODE_409;
            case shared::http::ECodes::kGoneValue: return oatpp::web::protocol::http::Status::CODE_410;
            case shared::http::ECodes::kLengthRequiredValue: return oatpp::web::protocol::http::Status::CODE_411;
            case shared::http::ECodes::kPreconditionFailedValue: return oatpp::web::protocol::http::Status::CODE_412;
            case shared::http::ECodes::kRequestEntityTooLargeValue: return oatpp::web::protocol::http::Status::CODE_413;
            case shared::http::ECodes::kRequestURITooLongValue: return oatpp::web::protocol::http::Status::CODE_414;
            case shared::http::ECodes::kUnsupportedMediaTypeValue: return oatpp::web::protocol::http::Status::CODE_415;
            case shared::http::ECodes::kRequestedrangeunsatisfiableValue: return oatpp::web::protocol::http::Status::CODE_416;
            case shared::http::ECodes::kExpectationfailedValue: return oatpp::web::protocol::http::Status::CODE_417;
            case shared::http::ECodes::kUnprocessableentityValue: return oatpp::web::protocol::http::Status::CODE_422;
            case shared::http::ECodes::kLockedValue: return oatpp::web::protocol::http::Status::CODE_423;
            case shared::http::ECodes::kMethodfailureValue: return oatpp::web::protocol::http::Status::CODE_424;
            case shared::http::ECodes::kUnorderedCollectionValue: return oatpp::web::protocol::http::Status::CODE_425;
            case shared::http::ECodes::kUpgradeRequiredValue: return oatpp::web::protocol::http::Status::CODE_426;
            case shared::http::ECodes::kPreconditionRequiredValue: return oatpp::web::protocol::http::Status::CODE_428;
            case shared::http::ECodes::kTooManyRequestsValue: return oatpp::web::protocol::http::Status::CODE_429;
            case shared::http::ECodes::kRequestHeaderFieldsTooLargeValue: return oatpp::web::protocol::http::Status::CODE_431;
            case shared::http::ECodes::kRetryWithValue: return oatpp::web::protocol::http::Status::CODE_449;
            case shared::http::ECodes::kUnavailableForLegalReasonsValue: return oatpp::web::protocol::http::Status::CODE_451;
            case shared::http::ECodes::kNoResponseValue: return oatpp::web::protocol::http::Status::CODE_444;
            case shared::http::ECodes::kInternalServerErrorValue: return oatpp::web::protocol::http::Status::CODE_500;
            case shared::http::ECodes::kNotImplementedValue: return oatpp::web::protocol::http::Status::CODE_501;
            case shared::http::ECodes::kBadGatewayOrProxyErrorValue: return oatpp::web::protocol::http::Status::CODE_502;
            case shared::http::ECodes::kServiceUnavailableValue: return oatpp::web::protocol::http::Status::CODE_503;
            case shared::http::ECodes::kGatewayTimeoutValue: return oatpp::web::protocol::http::Status::CODE_504;
            case shared::http::ECodes::kHTTPVersionnotsupportedValue: return oatpp::web::protocol::http::Status::CODE_505;
            case shared::http::ECodes::kVariantAlsoNegotiatesValue: return oatpp::web::protocol::http::Status::CODE_506;
            case shared::http::ECodes::kInsufficientstorageValue: return oatpp::web::protocol::http::Status::CODE_507;
            case shared::http::ECodes::kLoopdetectedValue: return oatpp::web::protocol::http::Status::CODE_508;
            case shared::http::ECodes::kBandwidthLimitExceededValue: return oatpp::web::protocol::http::Status::CODE_509;
            case shared::http::ECodes::kNotextendedValue: return oatpp::web::protocol::http::Status::CODE_510;
            case shared::http::ECodes::kNetworkauthenticationrequiredValue: return oatpp::web::protocol::http::Status::CODE_511;
            default:
               YADOMS_LOG(error) << "Invalid Rest error " << static_cast<unsigned>(error);
               return Status::CODE_500;
            }
         }

         std::function<boost::shared_ptr<rest::IRestAnswer>(std::map<std::string, std::string>, std::string)> m_handler;
      };

      void CWebServer::refreshRestRoutes(const std::shared_ptr<oatpp::web::server::HttpRouter>& httpRouter,
                                         const std::string& restKeywordBase) const
      {
         for (const auto& service : *m_restServices)
         {
            for (const auto& accessPoint : *service->accessPoints())
            {
               httpRouter->route(ToString(accessPoint->method()).c_str(),
                                 std::string("/" + restKeywordBase + "/" + accessPoint->path()).c_str(),
                                 std::make_shared<COatppRestHandler>(accessPoint->handler()));
            }
         }
      }
   } //namespace oatppServer
} //namespace web
