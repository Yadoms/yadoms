#pragma once
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         class CClient
         {
         public:
            CClient(boost::asio::io_service& ioService,
                    const std::string& searchTarget,
                    const std::chrono::seconds& timeout);
            virtual ~CClient() = default;

            std::set<std::string> getDescriptionUrls() const { return m_descriptionUrls; }

         private:
            static const boost::asio::ip::address MulticastAddress;
            static const int MulticastPort;

            void handleSendDiscoveryRequest(const boost::system::error_code& error);
            void handleDiscoveryTimeout(const boost::system::error_code& errorCode);
            void handleReadHeader(const boost::system::error_code& errorCode, size_t bytesReceived);
            void startAsynchronousSend();
            static std::string getMulticastSearchRequest(const std::string& searchTarget);

            void closeSocketAndTimer();

            boost::asio::ip::udp::endpoint m_endpoint;
            boost::asio::ip::udp::socket m_socket;
            boost::asio::steady_timer m_timer;
            std::string m_ipAddress;
            std::string m_deviceName;
            std::string m_serialNumber;
            std::string m_multicastSearchRequest;
            std::string m_xmlTagValue;
            std::chrono::seconds m_timeout;

            enum { kMaxlength = 2048 };

            char m_data[kMaxlength]{};

            std::set<std::string> m_descriptionUrls;
         };
      }
   }
}
