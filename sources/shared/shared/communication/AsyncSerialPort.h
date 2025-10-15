#pragma once
#include <chrono>
#include <thread>
#include "IAsyncPort.h"
#include "IReceiveBufferHandler.h"
#include "AsyncPortConnectionNotification.h"

namespace shared::communication
{
   //--------------------------------------------------------------
   /// \brief	This class manage a serial port
   //--------------------------------------------------------------
   class CAsyncSerialPort : public IAsyncPort
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] port                 Serial port name
      /// \param[in] baudrate             Baudrate (in bauds)
      /// \param[in] parity               Parity (see boost::asio::serial_port_base::parity::type for values)
      /// \param[in] characterSize        Character size (from 5 to 8)
      /// \param[in] stop_bits            Nb of stop bits (see boost::asio::serial_port_base::stop_bits::type for values)
      /// \param[in] flowControl          Flow control (see boost::asio::serial_port_base::flow_control::type for values)
      /// \param[in] connectRetryDelay    Delay between 2 connection retries
      /// \param[in] flushAtConnect       If true (default), flush serial port buffers before listening on port
      //--------------------------------------------------------------
      explicit CAsyncSerialPort(const std::string& port,
                                 const boost::asio::serial_port_base::baud_rate& baudrate = boost::asio::serial_port_base::baud_rate(9600),
                                 const boost::asio::serial_port_base::parity& parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                 const boost::asio::serial_port_base::character_size& characterSize = boost::asio::serial_port_base::character_size(8),
                                 const boost::asio::serial_port_base::stop_bits& stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                 const boost::asio::serial_port_base::flow_control& flowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                                 const std::chrono::milliseconds& connectRetryDelay = std::chrono::seconds(30),
                                 bool flushAtConnect = true);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CAsyncSerialPort();

      //--------------------------------------------------------------
      /// \brief	Set a timeout for blocking send (if flow control is used). See note below.
      /// \param[in] writeTimeout         The timeout to apply : should be inferior to plugin stop delay (10s), so 5s is recommended
      /// \note                           Only useful if flow control is used : When flow control is used, write is a blocking operation,
      ///                                 so may never return if slave doesn't answer (ie disconnected cable). Set a timeout to rise an error
      ///                                 instead of permanently blocking.
      //--------------------------------------------------------------
      void setWriteTimeout(const std::chrono::milliseconds& writeTimeout);

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
      /// \brief	Establish the connection
      /// \return true if connected, false else
      //--------------------------------------------------------------
      virtual bool connect();

      //--------------------------------------------------------------
      /// \brief	Close the connection
      //--------------------------------------------------------------
      virtual void disconnect();

      //--------------------------------------------------------------
      /// \brief	Try to connect asynchronously
      //--------------------------------------------------------------
      void tryConnect();

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
      void readCompleted(const boost::system::error_code& error,
                           std::size_t bytesTransferred);

      //--------------------------------------------------------------
      /// \brief	                     Notify the event handler for connection event : SUCCESS
      //--------------------------------------------------------------
      void notifyEventHandler() const;

      //--------------------------------------------------------------
      /// \brief	                           Notify the event handler for connection event : ERROR
      /// \param[in] i18nErrorMessage        The i18n string representing the error : "serialport.failToOpen" -> "Impossible d'ouvrir le port s�rie"
      //--------------------------------------------------------------
      void notifyEventHandler(const std::string & i18nErrorMessage) const;

      //--------------------------------------------------------------
      /// \brief	                           Notify the event handler for connection event : ERROR
      /// \param[in] i18nErrorMessage        The i18n string representing the error : "serialport.failToOpen" -> "Impossible d'ouvrir le port s�rie {{failPort}}"
      /// \param[in] m_i18nMessageParameters The i18n string parameters : ie: ["failPort", "COM1"]
      //--------------------------------------------------------------
      void notifyEventHandler(const std::string & i18nErrorMessage, const std::map<std::string, std::string> & m_i18nMessageParameters) const;


      //--------------------------------------------------------------
      /// \brief	                     Send buffer content
      /// \param[in] buffer            The buffer to send
      //--------------------------------------------------------------
      void sendBuffer(const boost::asio::const_buffer& buffer);
   private:
      //--------------------------------------------------------------
      /// \brief	boost:asio context
      //--------------------------------------------------------------
      boost::asio::io_context m_io;

      //--------------------------------------------------------------
      /// \brief	The boost serial port
      //--------------------------------------------------------------
      boost::asio::serial_port m_boostSerialPort;

      //--------------------------------------------------------------
      /// \brief	Serial port configuration
      //--------------------------------------------------------------
      std::string m_port;
      boost::asio::serial_port_base::baud_rate m_baudrate;
      boost::asio::serial_port_base::parity m_parity;
      boost::asio::serial_port_base::character_size m_characterSize;
      boost::asio::serial_port_base::stop_bits m_stop_bits;
      boost::asio::serial_port_base::flow_control m_flowControl;

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
      const std::chrono::milliseconds m_connectRetryDelay;

      //--------------------------------------------------------------
      /// \brief	Try to reconnect timer delay
      //--------------------------------------------------------------
      boost::asio::steady_timer m_connectRetryTimer;

      //--------------------------------------------------------------
      /// \brief	Thread for asynchronous operations
      //--------------------------------------------------------------
      boost::shared_ptr<std::thread> m_asyncThread;

      //--------------------------------------------------------------
      /// \brief	Flush serial port buffers before listening on port (just after connection)
      //--------------------------------------------------------------
      bool m_flushAtConnect;

      //--------------------------------------------------------------
      /// \brief	The write timeout (if flow control is used)
      //--------------------------------------------------------------
      std::chrono::milliseconds m_writeTimeout;
      bool m_writeTimeouted;
   };
}
