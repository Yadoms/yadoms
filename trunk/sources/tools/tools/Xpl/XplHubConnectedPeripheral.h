#pragma once

#include "stdafx.h"
#include "XplMessage.h"

class CXplHubConnectedPeripheral
{
public:

   //--------------------------------------------------------------
   /// \brief			Ctor, it innitialize the last seen time at now
   /// \param [in]   portNumber : The port number of the peripheral
   /// \param [in]   interval : The interval in minutes of hbeat is sent
   //--------------------------------------------------------------
   CXplHubConnectedPeripheral(boost::asio::io_service & io_service,  std::string ip, int portNumber, int interval);

   //--------------------------------------------------------------
   /// \brief			Dtor 
   //--------------------------------------------------------------
   ~CXplHubConnectedPeripheral();

   //--------------------------------------------------------------
   /// \brief			Obtains the port number of the peripheral
   /// \return       The port number
   //--------------------------------------------------------------
   int getPortNumber() const;

   //--------------------------------------------------------------
   /// \brief			Obtains the emission interval of sending hbeat from the peripheral
   /// \return       The interval
   //--------------------------------------------------------------
   int getInterval() const;

   //--------------------------------------------------------------
   /// \brief			Change the emission interval of sending hbeat from the peripheral
   /// \param [in]   interval : The new interval
   //--------------------------------------------------------------
   void setInterval(int interval);

   //--------------------------------------------------------------
   /// \brief			Obtains the time where we last seen the peripheral
   /// \return       The last seen time
   //--------------------------------------------------------------
   boost::posix_time::ptime getLastTimeSeen() const;

   //--------------------------------------------------------------
   /// \brief			Change the last seen time to now
   //--------------------------------------------------------------
   void updateLastTimeSeenFromNow();

   //--------------------------------------------------------------
   /// \brief			Change the last seen time 
   /// \param [in]   time : The new last seen time 
   //--------------------------------------------------------------
   void updateLastTimeSeen(boost::posix_time::ptime time);

   //--------------------------------------------------------------
   /// \brief			Send the message to the remoteEndpoint
   /// \param [in]   message : The message to send
   //--------------------------------------------------------------
   void sendMessage(const CXplMessage & message);

private:
   int m_portNumber;
   int m_interval;
   std::string m_ip;
   boost::asio::ip::udp::socket m_socket;
   boost::posix_time::ptime m_lastTimeSeen;
   boost::asio::ip::udp::endpoint m_remoteEndPoint;
};