#pragma once

#include "stdafx.h"
#include "XplHubConnectedPeripheral.h"

//TODO : COMMENTER

class CXplHub
{
public:
   CXplHub(const std::string & localIPOfTheInterfaceToUse);

private:
   void startReceive();
   void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
  
   void ioServiceRunner();

   void runCheckApplicationLifeCycleTimeout();

   void checkApplicationLifeCycle();

   boost::asio::io_service m_ioService;

   boost::asio::ip::udp::endpoint m_localEndPoint;
   boost::asio::ip::udp::endpoint m_remoteEndPoint;

   boost::asio::ip::udp::socket m_socket;
   boost::array<char, 1024> m_receiveBuffer;

   //map of discovered periph (port number, last time seen)
   std::vector<boost::shared_ptr<CXplHubConnectedPeripheral> > m_discoveredPeripherals;

   boost::asio::deadline_timer m_timer;

   static const int ApplicationLifeCycleTimeout = 60; //seconds
};

