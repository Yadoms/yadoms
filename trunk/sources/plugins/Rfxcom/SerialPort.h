#pragma once
#include "IPort.h"
#include "PortSubscription.hpp"

//--------------------------------------------------------------
/// \brief	This class manage a serial port
//--------------------------------------------------------------
class CSerialPort : public IPort
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
   CSerialPort(
      const std::string& port,
      boost::asio::serial_port_base::baud_rate baudrate = boost::asio::serial_port_base::baud_rate(9600),
      boost::asio::serial_port_base::parity parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
      boost::asio::serial_port_base::character_size characterSize = boost::asio::serial_port_base::character_size(8),
      boost::asio::serial_port_base::stop_bits stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
      boost::asio::serial_port_base::flow_control flowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CSerialPort();

   // IPort Implementation
   virtual bool connect();
   virtual void disconnect();
   virtual bool isConnected() const;
   virtual void subscribeConnectionState(shared::event::CEventHandler& forEventHandler, int forId);
   virtual void subscribeReceiveData(shared::event::CEventHandler& forEventHandler, int forId);
   virtual void flush();
   virtual void send(const boost::asio::const_buffer& buffer);
   virtual void asyncSend(const boost::asio::const_buffer& buffer);
   virtual void receive(boost::asio::mutable_buffer& buffer);
   // [END] IPort Implementation

protected:
   //--------------------------------------------------------------
   /// \brief	Try to connect asynchronously
   //--------------------------------------------------------------
   void tryConnect();

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
   /// \brief	                     End of write operation handler
   /// \param[in] error             Result of operation
   /// \param[in] bytesTransferred  Written bytes number
   //--------------------------------------------------------------
   void writeCompleted(const boost::system::error_code& error, std::size_t bytesTransferred);

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
   /// \brief	Connection state event subscription
   //--------------------------------------------------------------
   CPortSubscription<bool> m_connectionStateSubscription;

   //--------------------------------------------------------------
   /// \brief	Data receive event subscription
   //--------------------------------------------------------------
   CPortSubscription<boost::asio::const_buffer> m_receiveDataSubscription;

   //--------------------------------------------------------------
   /// \brief	Try to reconnect timer delay
   //--------------------------------------------------------------
   static const boost::posix_time::time_duration ConnectRetryDelay;

   //--------------------------------------------------------------
   /// \brief	Read buffer max size
   //--------------------------------------------------------------
   static const std::size_t ReadBufferMaxSize;

   //--------------------------------------------------------------
   /// \brief	Read buffer max size
   //--------------------------------------------------------------
   boost::shared_ptr<char> readBuffer;


   //TODO : options à rajouter ?
   //  - ConnectRetryDelay
   //  - ReadBufferMaxSize
};



