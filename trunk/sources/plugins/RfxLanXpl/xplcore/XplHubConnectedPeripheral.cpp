#include "stdafx.h"
#include "XplHubConnectedPeripheral.h"
#include "XplMessage.h"
#include "XplHelper.h"
#include "XplException.h"
#include <shared/Log.h>
#include <Poco/Net/NetworkInterface.h>

namespace xplcore
{


   CXplHubConnectedPeripheral::CXplHubConnectedPeripheral(Poco::Net::SocketAddress & sender, unsigned short portNumber, int interval, const std::string & debugName)
      : m_portNumber(portNumber), m_interval(interval), m_lastTimeSeen(boost::posix_time::second_clock::universal_time()), m_debugName(debugName)
   {
      m_socket.setReuseAddress(true);
      m_socket.setBroadcast(true);
      //m_socket.bind(Poco::Net::SocketAddress(sender.host(), portNumber));
      m_socket.connect(Poco::Net::SocketAddress(sender.host(), portNumber));
      YADOMS_LOG(debug) << "Peripheral " << debugName << " found @" << sender.host().toString();
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
      //boost::asio::buffer buf(message.toString());
      std::string buf = message.toString();
      m_socket.sendBytes(buf.c_str(), buf.size());
   }

} // namespace xplcore
