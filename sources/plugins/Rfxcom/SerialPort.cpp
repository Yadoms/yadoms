#include "stdafx.h"
#include "SerialPort.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "PortException.hpp"
#include "Buffer.hpp"


const boost::posix_time::time_duration CSerialPort::ConnectRetryDelay(boost::posix_time::seconds(30));
const std::size_t CSerialPort::ReadBufferMaxSize(512);

CSerialPort::CSerialPort(
   const std::string& port,
   boost::asio::serial_port_base::baud_rate baudrate,
   boost::asio::serial_port_base::parity parity,
   boost::asio::serial_port_base::character_size characterSize,
   boost::asio::serial_port_base::stop_bits stop_bits,
   boost::asio::serial_port_base::flow_control flowControl)
   :m_boostSerialPort(m_ioService),
   m_port(port), m_baudrate(baudrate), m_parity(parity), m_characterSize(characterSize), m_stop_bits(stop_bits), m_flowControl(flowControl),
   m_asyncReadBuffer(new unsigned char[ReadBufferMaxSize]), m_connectRetryTimer(m_ioService)
{
}

CSerialPort::~CSerialPort()
{
   stop();
}

void CSerialPort::start()
{
   if (!!m_asyncThread)
      return;  // Already started

   // Try to connect
   m_ioService.post(boost::bind(&CSerialPort::tryConnect, this));
   m_asyncThread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService)));
}

void CSerialPort::stop()
{
   if (!m_asyncThread)
      return;  // Already stopped

   disconnect();

   m_ioService.stop();
   m_asyncThread->join();
   m_asyncThread.reset();
}

bool CSerialPort::connect()
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

void CSerialPort::disconnect()
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

bool CSerialPort::isConnected() const
{
   return m_boostSerialPort.is_open();
}

void CSerialPort::subscribeConnectionState(shared::event::CEventHandler& forEventHandler, int forId)
{
   m_connectionStateSubscription.subscribe(forEventHandler, forId);
}

void CSerialPort::subscribeReceiveData(shared::event::CEventHandler& forEventHandler, int forId)
{
   m_receiveDataSubscription.subscribe(forEventHandler, forId);
}

void CSerialPort::setLogger(boost::shared_ptr<IPortLogger> logger)
{
   m_logger = logger;
}

void CSerialPort::flush()
{
   // Stop listening on the port if corresponding notifications are enabled
   if (m_receiveDataSubscription.hasSubscription())
   {
      m_boostSerialPort.cancel();
      //TODO attendre l'ack du cancel ?
   }

   receive();

   // Restart listening on the port if corresponding notifications are enabled
   if (m_receiveDataSubscription.hasSubscription())
      startRead();
}

void CSerialPort::reconnectTimerHandler(const boost::system::error_code& error)
{
   BOOST_ASSERT_MSG(error == 0, "Error code should be 0 here");

   tryConnect();
}

void CSerialPort::tryConnect()
{
   BOOST_ASSERT_MSG(!isConnected(), "Already connected");

   if (!connect())
   {
      // Fail to reconnect, retry after a certain delay
      m_connectRetryTimer.expires_from_now(ConnectRetryDelay);
      m_connectRetryTimer.async_wait(boost::bind(&CSerialPort::reconnectTimerHandler, this, boost::asio::placeholders::error));
      return;
   }

   // Connected
   m_connectionStateSubscription.notify(true);

   // Start listening on the port if corresponding notifications are enabled
   if (m_receiveDataSubscription.hasSubscription())
      startRead();
}

void CSerialPort::startRead()
{
   // Start an asynchronous read and call readCompleted when it completes or fails 
   m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.get(), ReadBufferMaxSize), 
      boost::bind(&CSerialPort::readCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CSerialPort::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is the normal stop
      if (error == boost::asio::error::operation_aborted)
         return;

      // Error ==> disconnecting
      YADOMS_LOG(error) << "Serial port read error : " << error.message();
      disconnect();
      m_connectionStateSubscription.notify(false);
      return;
   }

   // Read OK
   CByteBuffer buffer(m_asyncReadBuffer.get(), bytesTransferred);

   if (!!m_logger)
      m_logger->logReceived(buffer);

   // Send data
   m_receiveDataSubscription.notify(buffer);

   // Restart read
   startRead();
}

void CSerialPort::send(const CByteBuffer& buffer)
{
   try
   {
      if (!!m_logger)
         m_logger->logSent(buffer);

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

CByteBuffer CSerialPort::receive()
{
   try
   {
      boost::shared_ptr<unsigned char[]> readBuffer(new unsigned char[ReadBufferMaxSize]);   //TODO mettre readBuffer en membre pour plus de perfs ?
      std::size_t bytesTransferred = m_boostSerialPort.read_some(boost::asio::buffer(readBuffer.get(), ReadBufferMaxSize));

      BOOST_ASSERT_MSG(bytesTransferred != 0, "bytesTransferred should not be 0 at this point");

      CByteBuffer buffer(readBuffer.get(), bytesTransferred);
      if (!!m_logger)
         m_logger->logReceived(buffer);

      return buffer;
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

CByteBuffer CSerialPort::sendAndReceive(const CByteBuffer& buffer)
{
   // Stop listening on the port if corresponding notifications are enabled
   if (m_receiveDataSubscription.hasSubscription())
   {
      m_boostSerialPort.cancel();
      //TODO attendre l'ack du cancel ?
   }

   send(buffer);
   CByteBuffer receivedData = receive();

   // Restart listening on the port if corresponding notifications are enabled
   if (m_receiveDataSubscription.hasSubscription())
      startRead();

   return receivedData;
}


