#include "SsdpClient.h"
#include <boost/bind/bind.hpp>
#include <chrono>
#include <Poco/Net/IPAddress.h>
#include "shared/http/HttpMethods.h"
#include "HttpResponseHelper.h"

const boost::asio::ip::address CSsdpClient::MulticastAddress = boost::asio::ip::address_v4::from_string(
   "239.255.255.250");
const int CSsdpClient::MulticastPort = 1900;

CSsdpClient::CSsdpClient(boost::asio::io_service& ioService, const std::string& searchTarget,
                         const std::chrono::seconds& timeout)
   : m_endpoint(MulticastAddress, MulticastPort),
     m_socket(ioService, m_endpoint.protocol()),
     m_timer(ioService),
     m_multicastSearchRequest(getMulticastSearchRequest(searchTarget)),
     m_timeout(timeout)
{
   startAsynchronousSend();
}

void CSsdpClient::startAsynchronousSend()
{
   try
   {
      m_socket.set_option(
         boost::asio::ip::multicast::outbound_interface(
            boost::asio::ip::address_v4::from_string(Poco::Net::IPAddress().toString())));
   }
   catch (std::exception&)
   {
      throw std::runtime_error("Could not bind to the specified ip");
   }
   m_socket.async_send_to(boost::asio::buffer(m_multicastSearchRequest), m_endpoint,
                          boost::bind(&CSsdpClient::handleSendDiscoveryRequest, this,
                                      boost::asio::placeholders::error));
}

void CSsdpClient::handleSendDiscoveryRequest(const boost::system::error_code& errorCode)
{
   if (!errorCode)
   {
      m_socket.async_receive(boost::asio::buffer(m_data),
                             boost::bind(&CSsdpClient::handleReadHeader, this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));

      m_timer.expires_after(m_timeout);
      m_timer.async_wait(boost::bind(&CSsdpClient::handleDiscoveryTimeout, this,
                                     boost::asio::placeholders::error));
   }
}

void CSsdpClient::handleDiscoveryTimeout(const boost::system::error_code& errorCode)
{
   // boost::asio::error::operation_aborted : Normal behaviour-Invoked when we cancelled the timer
   if (!errorCode || errorCode == boost::asio::error::operation_aborted)
   {
      closeSocketAndTimer();
      return;
   }

   throw std::runtime_error("Error trying to receive data with ssdp discovery request");
}

void CSsdpClient::handleReadHeader(const boost::system::error_code& errorCode, size_t bytesReceived)
{
   if (errorCode && errorCode != boost::asio::error::operation_aborted)
   {
      removeDuplicates(m_descriptionUrls);
      removeEmptyStrings(m_descriptionUrls);
      closeSocketAndTimer();
      return;
   }

   const auto httpResponse = std::string(m_data);

   if (!CHttpResponseHelper::isValidHttpResponse(httpResponse))
   {
      throw std::runtime_error("Invalid response from the device");
   }

   if (!CHttpResponseHelper::isValidHttpStatusCode(httpResponse))
   {
      throw std::runtime_error("Response returned with a failed status code");
   }

   const std::string headerLocationName = "location";

   m_descriptionUrl = CHttpResponseHelper::getHttpResponseHeaderField(headerLocationName, httpResponse);

   m_descriptionUrls.push_back(m_descriptionUrl);

   m_socket.async_receive(boost::asio::buffer(m_data),
                          boost::bind(&CSsdpClient::handleReadHeader, this,
                                      boost::asio::placeholders::error,
                                      boost::asio::placeholders::bytes_transferred));
}

std::string CSsdpClient::getMulticastSearchRequest(const std::string& searchTarget)
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

void CSsdpClient::removeDuplicates(std::vector<std::string>& v)
{
   auto end = v.end();
   for (auto it = v.begin(); it != end; ++it)
   {
      end = std::remove(it + 1, end, *it);
   }

   v.erase(end, v.end());
}

void CSsdpClient::removeEmptyStrings(std::vector<std::string>& strings)
{
   const auto it = remove_if(strings.begin(), strings.end(), std::mem_fun_ref(&std::string::empty));
   strings.erase(it, strings.end());
}

void CSsdpClient::closeSocketAndTimer()
{
   m_socket.close();
   m_timer.cancel();
}
