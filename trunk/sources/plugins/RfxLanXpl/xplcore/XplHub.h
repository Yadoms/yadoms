#pragma once

#include "XplHubConnectedPeripheral.h"
#include <shared/ThreadBase.h>


namespace xplcore
{

   class CXplHub : public shared::CThreadBase
   {
   public:
      CXplHub(const std::string & localIPOfTheInterfaceToUse);

      virtual ~CXplHub();

      // CThreadBase override
      virtual bool stop();
      // {END] CThreadBase override

      //----------------------------------
      ///\brief   Update the filtering ips
      ///\param [in]   localIPOfTheInterfaceToUse  The ip to filter
      //----------------------------------
      void updateHubFilter(const std::string & localIPOfTheInterfaceToUse);

   private:
      //--------------------------------------------------------------
      /// \brief	   Check if a message is filtered
      //--------------------------------------------------------------
      bool isFiltered(CXplMessage & msg);


   private:
      void doWork();
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

      bool m_stopRequested;

      static const int ApplicationLifeCycleTimeout = 60; //seconds

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration content
      //--------------------------------------------------------------
      mutable boost::mutex m_configurationMutex;

   };
   
} // namespace xplcore
