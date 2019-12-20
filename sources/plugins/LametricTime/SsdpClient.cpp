#include "SsdpClient.h"
#include <boost/bind.hpp>
#include <chrono>
#include <Poco/Net/IPAddress.h>
#include "shared/http/HttpMethods.h"
#include "HttpResponseHelper.h"

const boost::asio::ip::address CSsdpClient::MulticastAddress = boost::asio::ip::address_v4::from_string(
	"239.255.255.250");
const int CSsdpClient::MulticastPort = 1900;

CSsdpClient::CSsdpClient(boost::asio::io_service& ioService, const std::string& searchTarget)
	: m_endpoint(MulticastAddress, MulticastPort),
	  m_socket(ioService, m_endpoint.protocol()),
	  m_timer(ioService)
{
	m_multicastSearchRequest = getMulticastSearchRequest(searchTarget);
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
		m_timer.expires_from_now(std::chrono::milliseconds(5000));
		m_timer.async_wait(boost::bind(&CSsdpClient::handleDiscoveryTimeout, this,
		                               boost::asio::placeholders::error));
	}
}

void CSsdpClient::handleDiscoveryTimeout(const boost::system::error_code& errorCode)
{
	if (errorCode == boost::asio::error::operation_aborted)
	{
		return;
		//Normal behaviour-Invoked when we cancelled the timer
	}
	if (errorCode)
	{
		throw std::runtime_error("Error trying to send ssdp discovery request");
	}
	if (!errorCode)
	{
		m_socket.close();
		throw std::runtime_error("No response from the device: wrong ip or no device listening on this network");
	}
}

void CSsdpClient::handleReadHeader(const boost::system::error_code& errorCode, size_t bytesReceived)
{
	if (errorCode)
	{
		throw std::runtime_error("Error trying to receive ssdp answer");
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

	const std::string headerLocationName = "LOCATION";

	m_descriptionUrl = CHttpResponseHelper::getHttpResponseHeaderField(headerLocationName, httpResponse);
	m_socket.close();
	m_timer.cancel();
}

std::string CSsdpClient::getMulticastSearchRequest(const std::string& searchTarget)
{
	std::stringstream os;
	os << "M-SEARCH * HTTP/1.1\r\n";
	os << "HOST: 239.255.255.250:1900\r\n";
	os << "MAN: \"ssdp:discover\"\r\n";
	os << "MX: 4\r\n";
	os << "ST: " + searchTarget + "\r\n";
	os << "\r\n";
	return os.str();
}
