#include "stdafx.h"
#include "AsyncSerialPort.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Peripherals.h>
#include "PortException.hpp"
#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      CAsyncSerialPort::CAsyncSerialPort(const std::string& port,
                                         boost::asio::serial_port_base::baud_rate baudrate,
                                         boost::asio::serial_port_base::parity parity,
                                         boost::asio::serial_port_base::character_size characterSize,
                                         boost::asio::serial_port_base::stop_bits stop_bits,
                                         boost::asio::serial_port_base::flow_control flowControl,
                                         boost::posix_time::time_duration connectRetryDelay,
                                         bool flushAtConnect)
         : m_boostSerialPort(m_ioService),
           m_port(port),
           m_baudrate(baudrate),
           m_parity(parity),
           m_characterSize(characterSize),
           m_stop_bits(stop_bits),
           m_flowControl(flowControl),
           m_asyncReadBuffer(512),
           m_connectStateEventHandler(nullptr),
           m_connectStateEventId(event::kNoEvent),
           m_connectRetryDelay(connectRetryDelay),
           m_connectRetryTimer(m_ioService),
           m_flushAtConnect(flushAtConnect)
      {
      }

      CAsyncSerialPort::~CAsyncSerialPort()
      {
         CAsyncSerialPort::stop();
      }

      void CAsyncSerialPort::setReceiveBufferMaxSize(std::size_t size)
      {
         m_asyncReadBuffer.resize(size);
      }

      void CAsyncSerialPort::start()
      {
         if (!!m_asyncThread)
            return; // Already started

         // Try to connect
         tryConnect();
         m_asyncThread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService)));
      }

      void CAsyncSerialPort::stop()
      {
         if (!m_asyncThread)
            return; // Already stopped

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

      void CAsyncSerialPort::subscribeForConnectionEvents(event::CEventHandler& forEventHandler,
                                                          int forId)
      {
         m_connectStateEventHandler = &forEventHandler;
         m_connectStateEventId = forId;
      }

      void CAsyncSerialPort::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
      {
         m_receiveBufferHandler = handler;
      }

      void CAsyncSerialPort::flush()
      {
         // Hardware flush
         CPeripherals::flushSerialPort(m_boostSerialPort);

         // Clear the receive buffer
         if (!!m_receiveBufferHandler)
            m_receiveBufferHandler->flush();
      }

      void CAsyncSerialPort::reconnectTimerHandler(const boost::system::error_code& error)
      {
         if (error)
            throw exception::CException("Error code should be 0 here");

         tryConnect();
      }

      void CAsyncSerialPort::tryConnect()
      {
         if (isConnected())
         {
            YADOMS_LOG(warning) << "Already connected";
         }
         else
         {
            if (!connect())
            {
               // Fail to reconnect, retry after a certain delay
               m_connectRetryTimer.expires_from_now(m_connectRetryDelay);
               m_connectRetryTimer.async_wait(boost::bind(&CAsyncSerialPort::reconnectTimerHandler, this, boost::asio::placeholders::error));
               return;
            }

            // Flush buffers if required
            if (m_flushAtConnect)
               flush();

            // Connected
            notifyEventHandler(true);
         }

         // Start listening on the port
         startRead();
      }

      void CAsyncSerialPort::startRead()
      {
         // Start an asynchronous read and call readCompleted when it completes or fails 
         m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.begin(), m_asyncReadBuffer.size()),
                                           boost::bind(&CAsyncSerialPort::readCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
      }

      void CAsyncSerialPort::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
      {
         if (error)
         {
            // boost::asio::error::operation_aborted is fired when stop is required
            if (error == boost::asio::error::operation_aborted)
               return; // Normal stop

            // Error ==> disconnecting
            YADOMS_LOG(error) << "Serial port read error : " << error.message();
            disconnect();
            notifyEventHandler(false);
            return;
         }

         // Read OK
         CByteBuffer buffer(bytesTransferred);
         memcpy(buffer.begin(), m_asyncReadBuffer.begin(), bytesTransferred);

         if (!!m_receiveBufferHandler)
            m_receiveBufferHandler->push(buffer);

         // Restart read
         startRead();
      }

      void CAsyncSerialPort::send(const CByteBuffer& buffer)
      {
         boost::asio::const_buffers_1 toSend(buffer.begin(), buffer.size());
         sendBuffer(toSend);
      }

      void CAsyncSerialPort::sendText(const std::string & content)
      {
         boost::asio::const_buffers_1 toSend = boost::asio::buffer(content);
         sendBuffer(toSend);
      }

      void CAsyncSerialPort::sendBuffer(boost::asio::const_buffers_1 & buffer)
      {
         try
         {
            m_boostSerialPort.write_some(buffer);
         }
         catch (boost::system::system_error& e)
         {
            // boost::asio::error::eof is the normal stop
            if (e.code() != boost::asio::error::eof)
            {
               YADOMS_LOG(error) << "Serial port write text error : " << e.what();
               disconnect();
            }

            notifyEventHandler(false);
            throw CPortException((e.code() == boost::asio::error::eof) ? CPortException::kConnectionClosed : CPortException::kConnectionError, e.what());
         }
      }


      void CAsyncSerialPort::notifyEventHandler(bool isConnected) const
      {
         if (m_connectStateEventHandler)
            m_connectStateEventHandler->postEvent<bool>(m_connectStateEventId, isConnected);
      }
   }
} // namespace shared::communication
