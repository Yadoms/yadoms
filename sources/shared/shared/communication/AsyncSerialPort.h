#pragma once
#include <shared/Export.h>
#include "IAsyncPort.h"
#include "IReceiveBufferHandler.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a serial port
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CAsyncSerialPort : public IAsyncPort
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
                                   const boost::posix_time::time_duration& connectRetryDelay = boost::posix_time::seconds(30),
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
         void setWriteTimeout(const boost::posix_time::time_duration& writeTimeout);

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
         /// \brief	                     Notify the event handler for connection event
         /// \param[in] isConnected       Connection state
         //--------------------------------------------------------------
         void notifyEventHandler(bool isConnected) const;

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
         const boost::posix_time::time_duration m_connectRetryDelay;

         //--------------------------------------------------------------
         /// \brief	Try to reconnect timer delay
         //--------------------------------------------------------------
         boost::asio::deadline_timer m_connectRetryTimer;

         //--------------------------------------------------------------
         /// \brief	Thread for asynchronous operations
         //--------------------------------------------------------------
         boost::shared_ptr<boost::thread> m_asyncThread;

         //--------------------------------------------------------------
         /// \brief	Flush serial port buffers before listening on port (just after connection)
         //--------------------------------------------------------------
         bool m_flushAtConnect;

         //--------------------------------------------------------------
         /// \brief	The write timeout (if flow control is used)
         //--------------------------------------------------------------
         boost::posix_time::time_duration m_writeTimeout;
         bool m_writeTimeouted;
      };
   }
} // namespace shared::communication
