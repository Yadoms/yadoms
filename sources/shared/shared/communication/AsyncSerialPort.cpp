#include "stdafx.h"
#include "AsyncSerialPort.h"
#include <shared/Log.h>
#include <shared/SerialPortHelper.h>
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
           m_writeTimeout(boost::date_time::pos_infin),
           m_writeTimeouted(false)
      {
      }

      CAsyncSerialPort::~CAsyncSerialPort()
      {
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

         m_ioService.stop();
         m_asyncThread->join();
         m_asyncThread.reset();
      }

      bool CAsyncSerialPort::connect()
      {
         // Open the port
         try
         {
            YADOMS_LOG(debug) << "Open " << m_port << "...";
            m_boostSerialPort.open(m_port);
         }
         catch (boost::system::system_error& e)
         {
            notifyEventHandler("asyncPort.serial.failToOpen", {{"port", m_port}});
            YADOMS_LOG(error) << " : Failed to open serial port : " << e.what();
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
         if (!isConnected())
            return;

         // Close the port
         try
         {
            m_boostSerialPort.cancel();
            YADOMS_LOG(debug) << " : m_boostSerialPort.close();";
            m_boostSerialPort.close();
         }
         catch (boost::system::system_error& e)
         {
            YADOMS_LOG(error) << " : Failed to close serial port " << e.what();
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
         CSerialPortHelper::flushSerialPort(m_boostSerialPort);

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
            YADOMS_LOG(warning) << " : Already connected";
         }
         else
         {
            if (!connect())
            {
               YADOMS_LOG(debug) << " : Fail to reconnect, retry in a while...";
               m_connectRetryTimer.expires_from_now(m_connectRetryDelay);
               m_connectRetryTimer.async_wait(boost::bind(&CAsyncSerialPort::reconnectTimerHandler, this,
                                                          boost::asio::placeholders::error));
               return;
            }

            // Flush buffers if required
            if (m_flushAtConnect)
               flush();

            //serial port opened
            notifyEventHandler();
         }

         // Start listening on the port
         startRead();
      }

      void CAsyncSerialPort::startRead()
      {
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
         if (error)
         {
            // boost::asio::error::operation_aborted is fired when stop is required
            if (error == boost::asio::error::operation_aborted)
            {
               if (m_writeTimeouted)
                  // Read operation was cancelled because of stop async write operation on timeout ==> must be restarted
                  startRead();
               return; // Normal stop
            }

            // Error ==> disconnecting
            YADOMS_LOG(error) << " : Serial port read error : " << error.message();
            disconnect();

            if (error == boost::asio::error::bad_descriptor)
               notifyEventHandler("asyncPort.serial.failToCommunicateWithHardware", {
                                     {"message", error.message()},
                                     {"code", (boost::format("%1%") % error.value()).str()}
                                  });
            else
               notifyEventHandler("asyncPort.serial.error", {
                                     {"message", error.message()},
                                     {"code", (boost::format("%1%") % error.value()).str()}
                                  });
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
         try
         {
            if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::none
               || m_writeTimeout == boost::date_time::pos_infin)
            {
               write(m_boostSerialPort, buffer);
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
               m_writeTimeouted = false;
               async_write(m_boostSerialPort, buffer,
                           [&evtHandler](const boost::system::error_code& ec,
                                         std::size_t bytes_transferred)
                           {
                              evtHandler.postEvent(kSendFinished, ec);
                           });

               switch (evtHandler.waitForEvents(m_writeTimeout))
               {
               case event::kTimeout:
                  {
                     m_writeTimeouted = true;
                     auto rc = boost::system::error_code(boost::system::errc::timed_out,
                                                         boost::asio::error::get_misc_category());
                     // Stop async operation
                     m_boostSerialPort.cancel(rc);
                     evtHandler.waitForEvents(m_writeTimeout);

                     // Fail to send data on serial port, timeout. Do as it was sent (should be handled by protocol)
                     YADOMS_LOG(warning) << " : Fail to send data on serial port, timeout";
                     return;
                  }
               case kSendFinished:
                  m_writeTimeouted = false;
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
               YADOMS_LOG(error) << " : Serial port send error : " << e.what();
               disconnect();
               notifyEventHandler("asyncPort.serial.connectionError");
               throw CPortException(CPortException::kConnectionError, e.what());
            }

            notifyEventHandler("asyncPort.serial.connectionClosed");
            throw CPortException(CPortException::kConnectionClosed, e.what());
         }
      }

      void CAsyncSerialPort::notifyEventHandler() const
      {
         if (m_connectStateEventHandler)
         {
            m_connectStateEventHandler->postEvent(m_connectStateEventId,
                                                  boost::make_shared<CAsyncPortConnectionNotification>());
         }
      }

      void CAsyncSerialPort::notifyEventHandler(const std::string& i18nErrorMessage) const
      {
         if (m_connectStateEventHandler)
         {
            m_connectStateEventHandler->postEvent(m_connectStateEventId,
                                                  boost::make_shared<CAsyncPortConnectionNotification
                                                  >(i18nErrorMessage));
         }
      }

      void CAsyncSerialPort::notifyEventHandler(const std::string& i18nErrorMessage,
                                                const std::map<std::string, std::string>& i18nMessageParameters) const
      {
         if (m_connectStateEventHandler)
         {
            m_connectStateEventHandler->postEvent(m_connectStateEventId,
                                                  boost::make_shared<CAsyncPortConnectionNotification>(
                                                     i18nErrorMessage, i18nMessageParameters));
         }
      }
   }
} // namespace shared::communication
