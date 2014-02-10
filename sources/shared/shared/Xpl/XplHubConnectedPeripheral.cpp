#include "stdafx.h"
#include "XplHubConnectedPeripheral.h"
#include "XplMessage.h"
#include "XplHelper.h"
#include "XplException.h"

//m_socket(m_ioService), 
CXplHubConnectedPeripheral::CXplHubConnectedPeripheral(boost::asio::io_service & io_service, std::string ip, unsigned short portNumber, int interval) 
   :  m_portNumber(portNumber), m_interval(interval), m_ip(ip), m_socket(io_service), m_lastTimeSeen(boost::posix_time::second_clock::universal_time())
{
   m_socket.open(boost::asio::ip::udp::v4());
   m_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
   m_socket.set_option(boost::asio::socket_base::broadcast(true));
   boost::asio::ip::udp::endpoint localEndPoint;
   if (!CXplHelper::getEndPointFromInterfaceIp(m_ip, localEndPoint))
   {
      //If we havn't found the given ip, error
      throw CXplException("Unable to find given ip : " + m_ip);
   }
   m_socket.bind(localEndPoint);

   m_remoteEndPoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(m_ip), m_portNumber);
}

CXplHubConnectedPeripheral::~CXplHubConnectedPeripheral()
{
   m_socket.close();
}

int CXplHubConnectedPeripheral::getPortNumber() const
{
   return m_portNumber;
}

int CXplHubConnectedPeripheral::getInterval() const
{
   return m_interval;
}

void CXplHubConnectedPeripheral::setInterval(int interval)
{
   m_interval = interval;
}

boost::posix_time::ptime CXplHubConnectedPeripheral::getLastTimeSeen() const
{
   return m_lastTimeSeen;
}

void CXplHubConnectedPeripheral::updateLastTimeSeenFromNow()
{
   m_lastTimeSeen = boost::posix_time::second_clock::universal_time();
}

void CXplHubConnectedPeripheral::updateLastTimeSeen(boost::posix_time::ptime time)
{
   m_lastTimeSeen = time;
}

void CXplHubConnectedPeripheral::sendMessage(const CXplMessage & message)
{
   m_socket.send_to(boost::asio::buffer(message.toString()), m_remoteEndPoint);
}