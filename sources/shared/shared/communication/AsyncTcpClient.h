#pragma once
#include "IAsyncPort.h"
#include "IReceiveBufferHandler.h"

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	This class manage a serial port
   //--------------------------------------------------------------
   class CAsyncTcpClient : public IAsyncPort
   {  
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] serverAddress        Server IP address or DNS name
      /// \param[in] serverPort           Server portNumber
      /// \param[in] connectRetryDelay    Delay between 2 connection retries
      //--------------------------------------------------------------
      CAsyncTcpClient(
         const std::string& serverAddress,
         const std::string& serverPort,
         boost::posix_time::time_duration connectRetryDelay = boost::posix_time::seconds(30));

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CAsyncTcpClient();

      // IAsyncPort Implementation
      void setReceiveBufferMaxSize(std::size_t size) override;
      void start() override;
      void stop() override;
      bool isConnected() const override;
      void subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId) override;
      void setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler) override;
      void send(const CByteBuffer& buffer) override;
      void sendText(const std::string & content) override;
      void flush() override;
      // [END] IAsyncPort Implementation

   protected:

      //--------------------------------------------------------------
      /// \brief	Close the connection
      //--------------------------------------------------------------
      void disconnect();

      //--------------------------------------------------------------
      /// \brief	Try to connect asynchronously
      //--------------------------------------------------------------
      void tryConnect();

      //--------------------------------------------------------------
      /// \brief	                     Handler called after end point was resolved
      /// \param[in] error             Error code (should be 0)
      /// \param[in] iterator          Selected end point
      //--------------------------------------------------------------
      void handleEndPointResolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iterator);

      //--------------------------------------------------------------
      /// \brief	                     Handler called when after connect
      /// \param[in] error             Error code (should be 0)
      //--------------------------------------------------------------
      void handleTryConnect(const boost::system::error_code& error);


      //--------------------------------------------------------------
      /// \brief	                     Handler called when connect retry timer expires
      /// \param[in] error             Error code (should be 0)
      //--------------------------------------------------------------
      void reconnectTimerHandler(const boost::system::error_code& error);

      //--------------------------------------------------------------
      /// \brief	Wait for something to read on the port
      //--------------------------------------------------------------
      void startRead();

      //--------------------------------------------------------------
      /// \brief	                     End of read operation handler
      /// \param[in] error             Result of operation
      /// \param[in] bytesTransferred  Read bytes number
      //--------------------------------------------------------------
      void readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred);

      //--------------------------------------------------------------
      /// \brief	                     Notify the event handler for connection event : SUCCESS
      //--------------------------------------------------------------
      void notifyEventHandler();

      //--------------------------------------------------------------
      /// \brief	                           Notify the event handler for connection event : ERROR
      /// \param[in] i18nErrorMessage        The i18n string representing the error : "serialport.failToOpen" -> "Impossible d'ouvrir le port ethernet"
      //--------------------------------------------------------------
      void notifyEventHandler(const std::string & i18nErrorMessage);

      //--------------------------------------------------------------
      /// \brief	                     Send buffer content
      /// \param[in] buffer            The buffer to send
      //--------------------------------------------------------------
      void sendBuffer(boost::asio::const_buffers_1 & buffer);

   private:
      //--------------------------------------------------------------
      /// \brief	boost:asio service
      //--------------------------------------------------------------
      boost::asio::io_service m_ioService;

      //--------------------------------------------------------------
      /// \brief	The boost socket port
      //--------------------------------------------------------------
      boost::asio::ip::tcp::socket m_boostSocket;

      //--------------------------------------------------------------
      /// \brief	The server address resolver
      //--------------------------------------------------------------
      boost::asio::ip::tcp::resolver m_serverAdressResolver;

      //--------------------------------------------------------------
      /// \brief	The server address resolver query
      //--------------------------------------------------------------
      boost::asio::ip::tcp::resolver::query m_serverAdressResolverQuery;

      //--------------------------------------------------------------
      /// \brief	Read buffer for asynchronous operations
      //--------------------------------------------------------------
      CByteBuffer m_asyncReadBuffer;

      //--------------------------------------------------------------
      /// \brief	The event handler to notify for connection events   
      //--------------------------------------------------------------
      event::CEventHandler* m_connectStateEventHandler;

      //--------------------------------------------------------------
      /// \brief	The event id to notify for connection events  
      //--------------------------------------------------------------
      int m_connectStateEventId;

      //--------------------------------------------------------------
      /// \brief	The receive buffer handler
      //--------------------------------------------------------------
      boost::shared_ptr<IReceiveBufferHandler> m_receiveBufferHandler;

      //--------------------------------------------------------------
      /// \brief	Try to reconnect timer delay
      //--------------------------------------------------------------
      const boost::posix_time::time_duration m_connectRetryDelay;

      //--------------------------------------------------------------
      /// \brief	Try to reconnect timer delay
      //--------------------------------------------------------------
      boost::asio::deadline_timer m_connectRetryTimer;

      //--------------------------------------------------------------
      /// \brief	Thread for asynchronous operations
      //--------------------------------------------------------------
      boost::shared_ptr<boost::thread> m_asyncThread;
   };

} } // namespace shared::communication


