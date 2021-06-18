#include "stdafx.h"
#include "WebServer.h"
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>
#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/web/server/HttpRouter.hpp>
#include <utility>
#include <shared/Log.h>

namespace web
{
   namespace oatppServer
   {
      class CHttpPages final : public oatpp::web::server::HttpRequestHandler
      {
         static const std::map<std::string, std::string> MimeTypes;

         static constexpr auto HeaderContentType = "Content-Type";
         static constexpr auto DefaultMimetype = "application/octet-stream";

         //TODO déplacer, et coder...
      public:
         CHttpPages(std::string name,
                    boost::filesystem::path siteLocation)
            : m_name(std::move(name)),
              m_siteLocation(std::move(siteLocation))
         {
         }

         ~CHttpPages() override = default;

         /**
          * Handle incoming request and return outgoing response.
          */
         std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override
         {
            auto page = request->getStartingLine().path.std_str();
            if (page == "/")
               page = "/index.html";
            const auto fullPath = m_siteLocation / page;

            const auto buffer = oatpp::base::StrBuffer::loadFromFile(fullPath.string().c_str());
            if (buffer == nullptr)
            {
               YADOMS_LOG(error) << "[" << m_name << "] => *** The resource \' " << page << "\' was not found";
               return ResponseFactory::createResponse(Status::CODE_404, getDefinedPage("The requested resource was not found!").c_str());
            }

            auto response = ResponseFactory::createResponse(Status::CODE_200, oatpp::String(buffer));
            response->putHeader(HeaderContentType, getMimeTypeFromPath(page));
            return response;
         }

         const char* getMimeTypeFromPath(const std::string& path) const
         {
            const auto find = MimeTypes.find(extension(boost::filesystem::path(path)));
            if (find == MimeTypes.end())
            {
               YADOMS_LOG(error) << "[" << m_name << "] => *** Mimetype not supported for page \'" << path << "\'";
               return DefaultMimetype;
            }
            return find->second.c_str();
         }

         static std::string getDefinedPage(const std::string& text)
         //TODO la page 404 (ou autre codes d'erreur) ne devrait-elle pas être gérée par le front ?
         {
            std::stringstream ss;
            ss << "<html>";
            ss << "<head>";
            ss << "<title>TVM</title>";
            ss << "</head>";
            ss << "<body>";
            ss << "<h1>";
            ss << text;
            ss << "</h1>";
            ss << "</body>";
            ss << "</html>";
            return ss.str();
         }


         //TODO : voir code DAT
         //void AddFileToCache(const std::string& filePath);
         //void ClearCache();
         //void AddVirtualPage(const std::string& page, const std::string& destination);
         //void ClearVirtualPages();

      private:
         const std::string m_name; //TODO utile ?
         const boost::filesystem::path m_siteLocation;
      };

      const std::map<std::string, std::string> CHttpPages::MimeTypes =
      {
         {".html", "text/html"},
         {".json", "text/json"},
         {".jpg", "image/jpeg"},
         {".jpeg", "image/jpeg"},
         {".png", "image/png"},
         {".gif", "image/gif"},
         {".ico", "image/x-icon"},
         {".css", "text/css"},
         {".js", "text/javascript"},
         {".xml", "text/xml"},
         {".bmp", "image/bmp"},
         {".pdf", "application/pdf"},
         {".wav", "audio/x-wav"},
         {".wave", "audio/x-wav"},
         {".svg", "image/svg+xml"},
         {".ttf", "font/ttf"}
      };

      CWebServer::CWebServer(const std::string& address,
                             bool useSSL,
                             unsigned short port,
                             unsigned short securedPort,
                             const std::string& docRoot,
                             const std::string& restKeywordBase,
                             const std::string& webSocketKeywordBase,
                             bool allowExternalAccess)
         : m_httpRequestHandlerFactory(std::make_shared<CHttpRequestHandlerFactory>())
      {
         oatpp::base::Environment::init();

         // Create Router component
         auto httpRouter = oatpp::web::server::HttpRouter::createShared();
         httpRouter->route("GET", "/", std::make_shared<CHttpPages>("Yadoms client", "www"));

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
   } //namespace oatppServer
} //namespace web
