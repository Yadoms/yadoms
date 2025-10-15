#include "stdafx.h"
#include "Client.h"
#include <boost/bind/bind.hpp>
#include <chrono>
#include <regex>
#include <Poco/Net/IPAddress.h>
#include <shared/Log.h>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         const boost::asio::ip::address CClient::MulticastAddress =
            boost::asio::ip::make_address_v4("239.255.255.250");
         const int CClient::MulticastPort = 1900;

         CClient::CClient(boost::asio::io_context& io, 
                          const std::string& searchTarget,
                          const std::chrono::seconds& timeout)
            : m_endpoint(MulticastAddress, MulticastPort),
              m_socket(io, m_endpoint.protocol()),
              m_timer(io),
              m_multicastSearchRequest(getMulticastSearchRequest(searchTarget)),
              m_timeout(timeout)
         {
            startAsynchronousSend();
         }

         void CClient::startAsynchronousSend()
         {
            try
            {
               m_socket.set_option(boost::asio::ip::multicast::outbound_interface(
                  boost::asio::ip::make_address_v4(Poco::Net::IPAddress().toString())));
            }
            catch (std::exception&)
            {
               throw std::runtime_error("Could not bind to the specified ip");
            }

            m_socket.async_send_to(boost::asio::buffer(m_multicastSearchRequest),
                                   m_endpoint,
                                   [this](const boost::system::error_code& ec, std::size_t bytes_transferred){
                                      handleSendDiscoveryRequest(ec, bytes_transferred);
                                   });
         }

         void CClient::handleSendDiscoveryRequest(const boost::system::error_code& errorCode,
                                                  const std::size_t bytesTransferred)
         {
            if (!errorCode)
            {
               m_socket.async_receive(
                  boost::asio::buffer(m_data),
                  [this](const boost::system::error_code& ec, std::size_t n){
                     handleReadHeader(ec, n);
                  });

               m_timer.expires_after(m_timeout);
               m_timer.async_wait([this](const boost::system::error_code& ec){
                  handleDiscoveryTimeout(ec);
               });
            }
         }

         void CClient::handleDiscoveryTimeout(const boost::system::error_code& errorCode)
         {
            // boost::asio::error::operation_aborted : Normal behaviour-Invoked when we cancelled the timer
            if (errorCode && errorCode != boost::asio::error::operation_aborted)
               throw std::runtime_error("Error trying to receive data with ssdp discovery request");

            closeSocketAndTimer();
         }

         void CClient::handleReadHeader(const boost::system::error_code& errorCode, size_t bytesReceived)
         {
            if (errorCode && errorCode != boost::asio::error::operation_aborted)
            {
               closeSocketAndTimer();
               return;
            }

            const auto httpResponse = std::string(m_data);

            if (getHttpCode(httpResponse) != ECodes::kOKValue)
               throw std::runtime_error("Response returned with a failed status code");

            const auto locationHeader = getHttpResponseHeaderField("location", httpResponse);
            if (!locationHeader.empty())
               m_descriptionUrls.insert(locationHeader);

            m_socket.async_receive(
               boost::asio::buffer(m_data),
               [this](const boost::system::error_code& ec, std::size_t n){
                  handleReadHeader(ec, n);
               });
         }

         std::string CClient::getMulticastSearchRequest(const std::string& searchTarget)
         {
            std::stringstream os;
            os << "M-SEARCH * HTTP/1.1\r\n";
            os << "HOST: 239.255.255.250:1900\r\n";
            os << "MAN: \"ssdp:discover\"\r\n";
            os << "MX: 10\r\n";
            os << "ST: " + searchTarget + "\r\n";
            os << "\r\n";
            return os.str();
         }

         void CClient::closeSocketAndTimer()
         {
            m_socket.close();
            m_timer.cancel();
         }

         std::string CClient::getHttpResponseHeaderField(const std::string& headerFieldName,
                                                         const std::string& httpResponse)
         {
            const std::regex pattern("^" + headerFieldName + ": ([^\\n$\\\\\\r]+)", std::regex::icase);
            std::smatch match;

            if (!std::regex_search(httpResponse, match, pattern))
               return std::string();

            return match[1].str();
         }

         ECodes CClient::getHttpCode(const std::string& httpResponse)
         {
            const std::regex pattern(R"(HTTP.*\s+(\d{3})\s)", std::regex::icase);
            std::smatch match;

            if (!std::regex_search(httpResponse, match, pattern))
               throw std::runtime_error("Invalid response from the device, http code not found");

            return ECodes(stoi(match[1].str()));
         }
      }
   }
}
