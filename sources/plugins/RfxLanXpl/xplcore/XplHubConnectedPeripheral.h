#pragma once

#include "XplMessage.h"
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>

namespace xplcore
{
   class CXplHubConnectedPeripheral
   {
   public:

      //--------------------------------------------------------------
      /// \brief			Ctor, it innitialize the last seen time at now
      /// \param [in]   portNumber : The port number of the peripheral
      /// \param [in]   interval : The interval in minutes of hbeat is sent
      /// \param [in]   debugName : Name to use for debug logs
      //--------------------------------------------------------------
      CXplHubConnectedPeripheral(Poco::Net::SocketAddress& sender,
                                 unsigned short portNumber,
                                 int interval,
                                 const std::string& debugName);

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
      void sendMessage(const CXplMessage& message);

   private:
      unsigned short m_portNumber;
      int m_interval;
      boost::posix_time::ptime m_lastTimeSeen;
      Poco::Net::DatagramSocket m_socket;
   };
} // namespace xplcore


