#include "stdafx.h"
#include "SerialPort.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "PortException.hpp"


const boost::posix_time::time_duration CSerialPort::ConnectRetryDelay(boost::posix_time::minutes(1));
const std::size_t CSerialPort::ReadBufferMaxSize(512);

CSerialPort::CSerialPort(
   boost::asio::io_service& iOService,
   const std::string& port,
   boost::asio::serial_port_base::baud_rate baudrate,
   boost::asio::serial_port_base::parity parity,
   boost::asio::serial_port_base::character_size characterSize,
   boost::asio::serial_port_base::stop_bits stop_bits,
   boost::asio::serial_port_base::flow_control flowControl)
   :boost::asio::serial_port(iOService),
   m_ioService(iOService),
   m_port(port), m_baudrate(baudrate), m_parity(parity), m_characterSize(characterSize), m_stop_bits(stop_bits), m_flowControl(flowControl),
   readBuffer(new char[ReadBufferMaxSize])
{
   // Try to connect
   tryConnect();
}

CSerialPort::~CSerialPort()
{
   disconnect();
}

bool CSerialPort::connect()
{
   // Open the port
   try
   {
      open(m_port);
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to open serial port " << e.what(); 
      return false;
   }

   // Set options to the port
   set_option(m_baudrate);
   set_option(m_parity);
   set_option(m_characterSize);
   set_option(m_stop_bits);
   set_option(m_flowControl);

   return true;
}

void CSerialPort::disconnect()
{
   if (!is_open())
      return;

   // Close the port
   try
   {
      close();
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to close serial port " << e.what();
      return;
   }
}

bool CSerialPort::isConnected() const
{
   return is_open();
}

void CSerialPort::subscribeConnectionState(boost::shared_ptr<shared::event::CEventHandler> forEventHandler, int forId)
{
   m_connectionStateSubscription.subscribe(forEventHandler, forId);
}

void CSerialPort::subscribeReceiveData(boost::shared_ptr<shared::event::CEventHandler> forEventHandler, int forId)
{
   m_receiveDataSubscription.subscribe(forEventHandler, forId);
}

void CSerialPort::flush()
{
   //TODO
}

void CSerialPort::tryConnect()
{
   BOOST_ASSERT_MSG(!isConnected(), "Already connected");

   if (!connect())
   {
      // Fail to reconnect, retry after a certain delay
      boost::asio::deadline_timer connectRetryTimer(m_ioService, ConnectRetryDelay);
      connectRetryTimer.async_wait(boost::bind(&CSerialPort::tryConnect, shared_from_this()));
      return;
   }

   // Connected
   m_connectionStateSubscription.notify(true);

   // Start listening on the port
   startRead();
}

void CSerialPort::startRead()
{
   // Start an asynchronous read and call readCompleted when it completes or fails 
   async_read_some(boost::asio::buffer(readBuffer.get(), ReadBufferMaxSize), 
      boost::bind(&CSerialPort::readCompleted, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CSerialPort::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is the normal stop
      if (error != boost::asio::error::operation_aborted)
      {
         YADOMS_LOG(error) << "Serial port read error : " << error.message();
         disconnect();
      }

      m_connectionStateSubscription.notify(false);
      return;
   }

   // Read OK

   // Send data
   m_receiveDataSubscription.notify(std::string(readBuffer.get(), bytesTransferred));

   // Restart read
   startRead();
}

void CSerialPort::send(const boost::asio::const_buffer& buffer)
{
   try
   {
      write_some(boost::asio::const_buffers_1(buffer));
   }
   catch (boost::system::system_error& e)
   {
      // boost::asio::error::eof is the normal stop
      if (e.code() != boost::asio::error::eof)
      {
         YADOMS_LOG(error) << "Serial port write error : " << e.what();
         disconnect();
      }

      m_connectionStateSubscription.notify(false);

      throw CPortException(
         (e.code() == boost::asio::error::eof) ? CPortException::kConnectionClosed : CPortException::kConnectionError,
         e.what());
   }
}

void CSerialPort::asyncSend(const boost::asio::const_buffer& buffer)
{
   // Start an asynchronous write and call write_complete when it completes or fails 
   async_write_some(boost::asio::buffer(buffer), 
      boost::bind(&CSerialPort::writeCompleted, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CSerialPort::writeCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is the normal stop
      if (error != boost::asio::error::operation_aborted)
      {
         YADOMS_LOG(error) << "Serial port write error : " << error.message();
         disconnect();
      }

      m_connectionStateSubscription.notify(false);
      return;
   }

   // Write OK
}

void CSerialPort::receive(boost::asio::mutable_buffer& buffer)
{
   try
   {
      read_some(boost::asio::mutable_buffers_1(buffer));
   }
   catch (boost::system::system_error& e)
   {
      // boost::asio::error::eof is the normal stop
      if (e.code() != boost::asio::error::eof)
      {
         YADOMS_LOG(error) << "Serial port read error : " << e.what();
         disconnect();
      }

      m_connectionStateSubscription.notify(false);

      throw CPortException(
         (e.code() == boost::asio::error::eof) ? CPortException::kConnectionClosed : CPortException::kConnectionError,
         e.what());
   }
}

