#pragma once
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>

class CSsdpClient
{
public:

	virtual ~CSsdpClient() = default;

	CSsdpClient(boost::asio::io_service& ioService, const std::string& searchTarget, const std::chrono::seconds& timeout);

	std::string getDescriptionUrl() const { return m_descriptionUrl; }
	
	std::vector<std::string> getDescriptionUrls() const { return m_descriptionUrls; }
private:
	const static boost::asio::ip::address MulticastAddress;
	const static int MulticastPort;

	void handleSendDiscoveryRequest(const boost::system::error_code& error);
	void handleDiscoveryTimeout(const boost::system::error_code& errorCode);
	void handleReadHeader(const boost::system::error_code& errorCode, size_t bytesReceived);
	void startAsynchronousSend();
	static std::string getMulticastSearchRequest(const std::string& searchTarget);

	static void removeDuplicates(std::vector<std::string>& v);
	static void removeEmptyStrings(std::vector<std::string>& strings);
	void closeSocketAndTimer();
	
	boost::asio::ip::udp::endpoint m_endpoint;
	boost::asio::ip::udp::socket m_socket;
	boost::asio::steady_timer m_timer;
	std::string m_descriptionUrl;
	std::string m_ipAddress;
	std::string m_deviceName;
	std::string m_serialNumber;
	std::string m_multicastSearchRequest;
	std::string m_xmlTagValue;
	std::chrono::seconds m_timeout;
	
	enum { kMaxlength = 1024 };

	char m_data[kMaxlength]{};

	std::vector<std::string> m_descriptionUrls;
};
