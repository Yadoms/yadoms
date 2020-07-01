#include "stdafx.h"
#include "Client.h"
#include <boost/bind/bind.hpp>
#include <chrono>
#include <regex>
#include <Poco/Net/IPAddress.h>
#include <shared/http/HttpMethods.h>
#include <shared/Log.h>

namespace shared
{
   namespace http
   {
      namespace ssdp
      {
         const boost::asio::ip::address CClient::MulticastAddress =
            boost::asio::ip::address_v4::from_string("239.255.255.250");
         const int CClient::MulticastPort = 1900;

         CClient::CClient(boost::asio::io_service& ioService, const std::string& searchTarget,
                          const std::chrono::seconds& timeout)
            : m_endpoint(MulticastAddress, MulticastPort),
              m_socket(ioService, m_endpoint.protocol()),
              m_timer(ioService),
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
                  boost::asio::ip::address_v4::from_string(Poco::Net::IPAddress().toString())));
            }
            catch (std::exception&)
            {
               throw std::runtime_error("Could not bind to the specified ip");
            }

            m_socket.async_send_to(boost::asio::buffer(m_multicastSearchRequest),
                                   m_endpoint,
                                   boost::bind(&CClient::handleSendDiscoveryRequest,
                                               this,
                                               boost::asio::placeholders::error));
         }

         void CClient::handleSendDiscoveryRequest(const boost::system::error_code& errorCode)
         {
            if (!errorCode)
            {
               m_socket.async_receive(boost::asio::buffer(m_data),
                                      boost::bind(&CClient::handleReadHeader, this,
                                                  boost::asio::placeholders::error,
                                                  boost::asio::placeholders::bytes_transferred));

               m_timer.expires_after(m_timeout);
               m_timer.async_wait(boost::bind(&CClient::handleDiscoveryTimeout, this,
                                              boost::asio::placeholders::error));
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

            static const std::string HeaderLocationName = "location";
            const auto locationHeader = getHttpResponseHeaderField(HeaderLocationName, httpResponse);
            if (!locationHeader.empty())
               m_descriptionUrls.insert(locationHeader);

            m_socket.async_receive(boost::asio::buffer(m_data),
                                   boost::bind(&CClient::handleReadHeader, this,
                                               boost::asio::placeholders::error,
                                               boost::asio::placeholders::bytes_transferred));
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
            const boost::regex pattern("^" + headerFieldName + ": ([^\\n$\\\\\\r]+)", boost::regex::icase);
            boost::smatch match;

            if (!regex_search(httpResponse, match, pattern))
               return std::string();

            return match[1].str();
         }

         ECodes CClient::getHttpCode(const std::string& httpResponse)
         {
            const boost::regex pattern("^HTTP.*(\\d{3})", boost::regex::icase);
            boost::smatch match;

            if (!regex_search(httpResponse, match, pattern))
               throw std::runtime_error("Invalid response from the device, http code not found");

            return ECodes(stoi(match[1].str()));
         }
      }
   }
}
