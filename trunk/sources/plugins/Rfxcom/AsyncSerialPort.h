#pragma once
#include "IAsyncPort.h"
#include "PortSubscription.hpp"

//--------------------------------------------------------------
/// \brief	This class manage a serial port
//--------------------------------------------------------------
class CAsyncSerialPort : public IAsyncPort
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] port              Serial port name
   /// \param[in] baudrate          Baudrate (in bauds)
   /// \param[in] parity            Parity (see boost::asio::serial_port_base::parity::type for values)
   /// \param[in] characterSize     Character size (from 5 to 8)
   /// \param[in] stop_bits         Nb of stop bits (see boost::asio::serial_port_base::stop_bits::type for values)
   /// \param[in] flowControl       Flow control (see boost::asio::serial_port_base::flow_control::type for values)
   //--------------------------------------------------------------
   CAsyncSerialPort(
      const std::string& port,
      boost::asio::serial_port_base::baud_rate baudrate = boost::asio::serial_port_base::baud_rate(9600),
      boost::asio::serial_port_base::parity parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
      boost::asio::serial_port_base::character_size characterSize = boost::asio::serial_port_base::character_size(8),
      boost::asio::serial_port_base::stop_bits stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
      boost::asio::serial_port_base::flow_control flowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CAsyncSerialPort();

   // IAsyncPort Implementation
   virtual void setReceiveBufferSize(std::size_t size);
   virtual void start();
   virtual void stop();
   virtual bool isConnected() const;
   virtual void subscribeConnectionState(shared::event::CEventHandler& forEventHandler, int forId);
   virtual void subscribeReceiveData(shared::event::CEventHandler& forEventHandler, int forId);
   virtual void setLogger(boost::shared_ptr<IPortLogger> logger);
   virtual void flush();
   virtual void send(const CByteBuffer& buffer);
   // [END] IAsyncPort Implementation

   //--------------------------------------------------------------
   /// \brief	Establish the connection
   /// \return true if connected, false else
   //--------------------------------------------------------------
   virtual bool connect();

   //--------------------------------------------------------------
   /// \brief	Close the connection
   //--------------------------------------------------------------
   virtual void disconnect();

protected:
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
   void readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred);

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
   /// \brief	Read buffer max size
   //--------------------------------------------------------------
   std::size_t m_readBufferMaxSize;

   //--------------------------------------------------------------
   /// \brief	Read buffer for asynchronous operations
   //--------------------------------------------------------------
   boost::shared_ptr<unsigned char[]> m_asyncReadBuffer;

   //--------------------------------------------------------------
   /// \brief	Read buffer for synchronous operations
   //--------------------------------------------------------------
   boost::shared_ptr<unsigned char[]> m_readBuffer;

   //--------------------------------------------------------------
   /// \brief	Connection state event subscription
   //--------------------------------------------------------------
   CPortSubscription<bool> m_connectionStateSubscription;

   //--------------------------------------------------------------
   /// \brief	Data receive event subscription
   //--------------------------------------------------------------
   CPortSubscription<CByteBuffer> m_receiveDataSubscription;

   //--------------------------------------------------------------
   /// \brief	Try to reconnect timer delay
   //--------------------------------------------------------------
   static const boost::posix_time::time_duration ConnectRetryDelay;

   //--------------------------------------------------------------
   /// \brief	Try to reconnect timer delay
   //--------------------------------------------------------------
   boost::asio::deadline_timer m_connectRetryTimer;

   //--------------------------------------------------------------
   /// \brief	Thread for asynchronous operations
   //--------------------------------------------------------------
   boost::shared_ptr<boost::thread> m_asyncThread;

   //--------------------------------------------------------------
   /// \brief	Logger to use (can be null to disable log)
   //--------------------------------------------------------------
   boost::shared_ptr<IPortLogger> m_logger;

   //--------------------------------------------------------------
   /// \brief	Flag when flush is in progress
   //--------------------------------------------------------------
   bool m_flushInProgress;


   //TODO : options � rajouter ?
   //  - ConnectRetryDelay
   //  - ReadBufferMaxSize
};



