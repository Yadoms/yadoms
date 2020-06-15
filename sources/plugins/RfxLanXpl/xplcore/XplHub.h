#pragma once

#include "XplHubConnectedPeripheral.h"
#include <shared/ThreadBase.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/SharedPtr.h>

namespace xplcore
{
   class CXplHub : public shared::CThreadBase
   {
   public:
      explicit CXplHub(const std::string& localIPOfTheInterfaceToUse);

      virtual ~CXplHub();

      // CThreadBase override
      void doWork() override;
      void stop() override;
      // {END] CThreadBase override

   private:
      //--------------------------------------------------------------
      /// \brief			Prepare an application life cycle
      //--------------------------------------------------------------
      void runCheckApplicationLifeCycleTimeout();

      //--------------------------------------------------------------
      /// \brief			Check an application life cycle
      //--------------------------------------------------------------
      void checkApplicationLifeCycle();

      //--------------------------------------------------------------
      /// \brief			Treat an xpl message received
      /// \param [in]   msg      The message
      /// \param [in]   sender   The sender
      //--------------------------------------------------------------
      void onXplMessageReceived(CXplMessage& msg,
                                Poco::Net::SocketAddress& sender);
      //--------------------------------------------------------------
      /// \brief			Treat a received heartbeat message
      /// \param [in]   msg      The message
      /// \param [in]   sender   The sender
      //--------------------------------------------------------------
      void manageReceivedHeartbeatMessage(const CXplMessage& hbeatMessage,
                                          Poco::Net::SocketAddress& sender);

      //--------------------------------------------------------------
      /// \brief			Broadcast a message to all discovered peripherals
      /// \param [in]   message : The message to send
      //--------------------------------------------------------------
      void broadcastMessage(CXplMessage& hbeatMessage);


      //--------------------------------------------------------------
      /// \brief			Discovered peripherals
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<CXplHubConnectedPeripheral> > m_discoveredPeripherals;

      //--------------------------------------------------------------
      /// \brief			The next heartbeat time
      //--------------------------------------------------------------
      boost::posix_time::ptime m_nextHeartbeatTime;

      //--------------------------------------------------------------
      /// \brief			The listening socket
      //--------------------------------------------------------------
      Poco::Net::DatagramSocket m_socket;

      bool m_stopHubRequested;

      //--------------------------------------------------------------
      /// \brief			The application life cycle timeout (60 seconds)
      //--------------------------------------------------------------      
      static int ApplicationLifeCycleTimeout; //seconds
   };
} // namespace xplcore


