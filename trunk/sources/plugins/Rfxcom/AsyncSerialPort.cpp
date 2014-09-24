#include "stdafx.h"
#include "AsyncSerialPort.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "PortException.hpp"
#include "Buffer.hpp"


CAsyncSerialPort::CAsyncSerialPort(
   const std::string& port,
   boost::shared_ptr<IReceiveBufferHandler> receiveBufferHandler,
   boost::asio::serial_port_base::baud_rate baudrate,
   boost::asio::serial_port_base::parity parity,
   boost::asio::serial_port_base::character_size characterSize,
   boost::asio::serial_port_base::stop_bits stop_bits,
   boost::asio::serial_port_base::flow_control flowControl,
   boost::posix_time::time_duration connectRetryDelay)
   :m_boostSerialPort(m_ioService),
   m_port(port),
   m_receiveBufferHandler(receiveBufferHandler),
   m_baudrate(baudrate), m_parity(parity), m_characterSize(characterSize), m_stop_bits(stop_bits), m_flowControl(flowControl),
   m_readBufferMaxSize(512),
   m_asyncReadBuffer(new unsigned char[m_readBufferMaxSize]),
   m_readBuffer(new unsigned char[m_readBufferMaxSize]),
   m_connectRetryTimer(m_ioService),
   m_connectRetryDelay(connectRetryDelay)
{
   BOOST_ASSERT_MSG(!!receiveBufferHandler, "A receive buffer handler must be specified");
}

CAsyncSerialPort::~CAsyncSerialPort()
{
   stop();
}

void CAsyncSerialPort::setReceiveBufferSize(std::size_t size)
{
   m_readBufferMaxSize = size;
   m_asyncReadBuffer.reset(new unsigned char[m_readBufferMaxSize]);
   m_readBuffer.reset(new unsigned char[m_readBufferMaxSize]);
}

void CAsyncSerialPort::start()
{
   if (!!m_asyncThread)
      return;  // Already started

   // Try to connect
   m_ioService.post(boost::bind(&CAsyncSerialPort::tryConnect, this));
   m_asyncThread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService)));
}

void CAsyncSerialPort::stop()
{
   if (!m_asyncThread)
      return;  // Already stopped

   disconnect();

   m_ioService.stop();
   m_asyncThread->join();
   m_asyncThread.reset();
}

bool CAsyncSerialPort::connect()
{
   // Open the port
   try
   {
      m_boostSerialPort.open(m_port);
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to open serial port " << e.what(); 
      return false;
   }

   // Set options to the port
   m_boostSerialPort.set_option(m_baudrate);
   m_boostSerialPort.set_option(m_parity);
   m_boostSerialPort.set_option(m_characterSize);
   m_boostSerialPort.set_option(m_stop_bits);
   m_boostSerialPort.set_option(m_flowControl);

   return true;
}

void CAsyncSerialPort::disconnect()
{
   if (!m_boostSerialPort.is_open())
      return;

   // Close the port
   try
   {
      m_boostSerialPort.close();
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to close serial port " << e.what();
      return;
   }
}

bool CAsyncSerialPort::isConnected() const
{
   return m_boostSerialPort.is_open();
}

void CAsyncSerialPort::subscribeConnectionState(shared::event::CEventHandler& forEventHandler, int forId)
{
   m_connectionStateSubscription.subscribe(forEventHandler, forId);
}

void CAsyncSerialPort::flush()
{
   m_receiveBufferHandler->flush();
}

void CAsyncSerialPort::reconnectTimerHandler(const boost::system::error_code& error)
{
   BOOST_ASSERT_MSG(error == 0, "Error code should be 0 here");

   tryConnect();
}

void CAsyncSerialPort::tryConnect()
{
   BOOST_ASSERT_MSG(!isConnected(), "Already connected");

   if (!connect())
   {
      // Fail to reconnect, retry after a certain delay
      m_connectRetryTimer.expires_from_now(m_connectRetryDelay);
      m_connectRetryTimer.async_wait(boost::bind(&CAsyncSerialPort::reconnectTimerHandler, this, boost::asio::placeholders::error));
      return;
   }

   // Connected
   m_connectionStateSubscription.notify(true);

   // Start listening on the port
   startRead();
}

void CAsyncSerialPort::startRead()
{
   // Start an asynchronous read and call readCompleted when it completes or fails 
   m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.get(), m_readBufferMaxSize), 
      boost::bind(&CAsyncSerialPort::readCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CAsyncSerialPort::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is fired when stop is required
      if (error == boost::asio::error::operation_aborted)
         return;     // Normal stop

      // Error ==> disconnecting
      YADOMS_LOG(error) << "Serial port read error : " << error.message();
      disconnect();
      m_connectionStateSubscription.notify(false);
      return;
   }

   // Read OK
   CByteBuffer buffer(m_asyncReadBuffer.get(), bytesTransferred);

   m_receiveBufferHandler->push(buffer);

   // Restart read
   startRead();
}

void CAsyncSerialPort::send(const CByteBuffer& buffer)
{
   try
   {
      m_boostSerialPort.write_some(boost::asio::const_buffers_1(buffer.content(), buffer.size()));
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