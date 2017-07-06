#include "stdafx.h"
#include "AsyncSerialPort.h"
#include <shared/Log.h>
#include <shared/Peripherals.h>
#include "PortException.hpp"
#include "Buffer.hpp"

namespace shared
{
   namespace communication
   {
      CAsyncSerialPort::CAsyncSerialPort(const std::string& port,
                                         const boost::asio::serial_port_base::baud_rate& baudrate,
                                         const boost::asio::serial_port_base::parity& parity,
                                         const boost::asio::serial_port_base::character_size& characterSize,
                                         const boost::asio::serial_port_base::stop_bits& stop_bits,
                                         const boost::asio::serial_port_base::flow_control& flowControl,
                                         const boost::posix_time::time_duration& connectRetryDelay,
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
           m_flushAtConnect(flushAtConnect),
           m_writeTimeout(boost::date_time::pos_infin)
      {
         YADOMS_LOG(debug) << "CAsyncSerialPort::CAsyncSerialPort()";//TODO virer
      }

      CAsyncSerialPort::~CAsyncSerialPort()
      {
         YADOMS_LOG(debug) << "CAsyncSerialPort::~CAsyncSerialPort()";//TODO virer
         CAsyncSerialPort::stop();
      }

      void CAsyncSerialPort::setWriteTimeout(const boost::posix_time::time_duration& writeTimeout)
      {
         m_writeTimeout = writeTimeout;
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
         m_asyncThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ioService));
      }

      void CAsyncSerialPort::stop()
      {
         if (!m_asyncThread)
            return; // Already stopped

         disconnect();

         YADOMS_LOG(debug) << "m_ioService.stop();";//TODO virer
         m_ioService.stop();
         m_asyncThread->join();
         m_asyncThread.reset();
      }

      bool CAsyncSerialPort::connect()
      {
         YADOMS_LOG(debug) << "CAsyncSerialPort::connect();";//TODO virer
         // Open the port
         try
         {

            YADOMS_LOG(debug) << "m_boostSerialPort.open();";
            m_boostSerialPort.open(m_port);
         }
         catch (boost::system::system_error& e)
         {
            YADOMS_LOG(error) << "Failed to open serial port : " << e.what();
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
         YADOMS_LOG(debug) << "CAsyncSerialPort::disconnect(); isConnected = " << isConnected();//TODO virer
         if (!isConnected())
            return;

         // Close the port
         try
         {
            YADOMS_LOG(debug) << "m_boostSerialPort.cancel();";//TODO virer
            m_boostSerialPort.cancel();
            YADOMS_LOG(debug) << "m_boostSerialPort.close();";
            m_boostSerialPort.close();
         }
         catch (boost::system::system_error& e)
         {
            YADOMS_LOG(error) << "Failed to close serial port " << e.what();
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
         YADOMS_LOG(debug) << "CAsyncSerialPort::flush();";//TODO virer

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
         YADOMS_LOG(debug) << "CAsyncSerialPort::tryConnect();";//TODO virer

         if (isConnected())
         {
            YADOMS_LOG(warning) << "Already connected";
         }
         else
         {
            if (!connect())
            {
               YADOMS_LOG(debug) << "Fail to reconnect, retry after a certain delay";
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
         YADOMS_LOG(debug) << "CAsyncSerialPort::startRead();";//TODO virer
         // Start an asynchronous read and call readCompleted when it completes or fails 
         m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.begin(),
                                                               m_asyncReadBuffer.size()),
                                           boost::bind(&CAsyncSerialPort::readCompleted,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
      }

      void CAsyncSerialPort::readCompleted(const boost::system::error_code& error,
                                           std::size_t bytesTransferred)
      {
         YADOMS_LOG(debug) << "CAsyncSerialPort::readCompleted(error=" << error << ", bytesTransferred=" << bytesTransferred << ");";//TODO virer
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

      void CAsyncSerialPort::sendText(const std::string& content)
      {
         auto toSend = boost::asio::buffer(content);
         sendBuffer(toSend);
      }

      void CAsyncSerialPort::sendBuffer(boost::asio::const_buffers_1& buffer)
      {
         YADOMS_LOG(debug) << "CAsyncSerialPort::sendBuffer();";//TODO virer
         try
         {
            if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::none
               || m_writeTimeout == boost::date_time::pos_infin)
            {
               YADOMS_LOG(debug) << "m_boostSerialPort.write_some();";//TODO virer
               m_boostSerialPort.write_some(buffer);
            }
            else
            {
               // Flow control is used, so write can block.
               // In this case, use async write and apply timeout
               event::CEventHandler evtHandler;
               enum
                  {
                     kSendFinished = event::kUserFirstId
                  };
               YADOMS_LOG(debug) << "m_boostSerialPort.async_write_some();";//TODO virer
               m_boostSerialPort.async_write_some(buffer,
                                                  [&evtHandler](const boost::system::error_code& ec,
                                                     std::size_t bytes_transferred)
                                                  {
                                                     YADOMS_LOG(debug) << "evtHandler.postEvent(kSendFinished, " << ec << ");";//TODO virer
                                                     evtHandler.postEvent(kSendFinished, ec);
                                                  });

               switch (evtHandler.waitForEvents(m_writeTimeout))
               {
               case event::kTimeout:
                  {
                     YADOMS_LOG(debug) << "m_boostSerialPort.async_write_some() ==> Timeout";//TODO virer
                     auto rc = boost::system::error_code(boost::system::errc::timed_out,
                                                         boost::asio::error::get_misc_category());
                     // Stop async operation
                     m_boostSerialPort.cancel(rc); //TODO attention, ça arrête aussi l'opération de lecture (qui n'est pas redémarrée)
                     auto event = evtHandler.waitForEvents(m_writeTimeout);
                     if (event != kSendFinished)
                        throw std::runtime_error("Fail to stop async send on serial port : " + std::to_string(event));

                     // Fail to send data on serial port, timeout. Do as it was sent (should be handled by protocol)
                     YADOMS_LOG(warning) << "Fail to send data on serial port, timeout";
                     return;
                     throw boost::system::system_error(boost::system::error_code(boost::system::errc::timed_out,
                                                                                 boost::asio::error::get_misc_category()),
                                                       "Fail to send data on serial port, timeout");//TODO virer
                  }
               case kSendFinished:
                  YADOMS_LOG(debug) << "m_boostSerialPort.async_write_some() ==> OK";//TODO virer
                  return;
               default:
                  throw std::runtime_error("Unexpected event " + std::to_string(evtHandler.getEventId()));
               }
            }
         }
         catch (boost::system::system_error& e)
         {
            // boost::asio::error::eof is the normal stop
            if (e.code() != boost::asio::error::eof)
            {
               YADOMS_LOG(error) << "Serial port send error : " << e.what();
               disconnect();
               notifyEventHandler(false);
               throw CPortException(CPortException::kConnectionError, e.what());
            }

            YADOMS_LOG(debug) << "e.code() == boost::asio::error::eof ==> normal stop";//TODO virer
            notifyEventHandler(false);
            throw CPortException(CPortException::kConnectionClosed, e.what());
         }
      }


      void CAsyncSerialPort::notifyEventHandler(bool isConnected) const
      {
         if (m_connectStateEventHandler)
            m_connectStateEventHandler->postEvent(m_connectStateEventId,
                                                  isConnected);
      }
   }
} // namespace shared::communication


