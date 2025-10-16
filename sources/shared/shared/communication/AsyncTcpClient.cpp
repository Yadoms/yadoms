#include "stdafx.h"
#include "AsyncTcpClient.h"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include "PortException.hpp"
#include "Buffer.hpp"
#include "AsyncPortConnectionNotification.h"

namespace shared { namespace communication {

CAsyncTcpClient::CAsyncTcpClient(
   const std::string& serverAddress,
   const std::string& serverPort,
   std::chrono::seconds connectRetryDelay)
   :m_boostSocket(m_io),
   m_serverAdressResolver(m_io),
   m_serverAddress(serverAddress),
   m_serverPort(serverPort),
   m_asyncReadBuffer(512),
   m_connectStateEventHandler(NULL),
   m_connectStateEventId(event::kNoEvent),
   m_connectRetryDelay(connectRetryDelay),
   m_connectRetryTimer(m_io)
{
}

CAsyncTcpClient::~CAsyncTcpClient()
{
   stop();
}

void CAsyncTcpClient::setReceiveBufferMaxSize(std::size_t size)
{
   m_asyncReadBuffer.resize(size);
}

void CAsyncTcpClient::start()
{
   if (!!m_asyncThread)
      return;  // Already started

   // Try to connect
   tryConnect();
   m_asyncThread.reset(new std::thread([this]{ m_io.run(); }));
}

void CAsyncTcpClient::stop()
{
   if (!m_asyncThread)
      return;  // Already stopped

   disconnect();

   m_io.stop();
   m_asyncThread->join();
   m_asyncThread.reset();
}

void CAsyncTcpClient::disconnect()
{
   if (!m_boostSocket.is_open())
      return;

   // Close the port
   try
   {
      m_boostSocket.close();
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to close socket " << e.what();
      return;
   }
}

bool CAsyncTcpClient::isConnected() const
{
   return m_boostSocket.is_open();
}

void CAsyncTcpClient::subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId)
{
   m_connectStateEventHandler = &forEventHandler;
   m_connectStateEventId = forId;
}

void CAsyncTcpClient::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
{
   m_receiveBufferHandler = handler;
}

void CAsyncTcpClient::flush()
{
   if (!!m_receiveBufferHandler)
      m_receiveBufferHandler->flush();
}

void CAsyncTcpClient::reconnectTimerHandler(const boost::system::error_code& error)
{
   if (error.value() != 0)
      throw exception::CException("Error code should be 0 here");

   tryConnect();
}

void CAsyncTcpClient::tryConnect()
{
   if (isConnected())
      YADOMS_LOG(warning) << "Already connected";

   m_serverAdressResolver.async_resolve(
      m_serverAddress, m_serverPort,
      [this](const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::results_type results){
         handleEndPointResolve(ec, results);
      });
}

void CAsyncTcpClient::handleEndPointResolve(const boost::system::error_code& error,
                                            boost::asio::ip::tcp::resolver::results_type results)
{
   if (error)
   {
      // Fail to reconnect, retry after a certain delay
      m_connectRetryTimer.expires_after(m_connectRetryDelay);
      m_connectRetryTimer.async_wait([this](const boost::system::error_code& ec){ reconnectTimerHandler(ec); });
      return;
   }

   // End point is solved, try to connect now
   boost::asio::async_connect(
      m_boostSocket,
      results,
      [this](const boost::system::error_code& ec, const boost::asio::ip::tcp::endpoint&){
         handleTryConnect(ec);
      });
}

void CAsyncTcpClient::handleTryConnect(const boost::system::error_code& error)
{
   if (error)
   {
      // Fail to reconnect, retry after a certain delay
      m_connectRetryTimer.expires_after(m_connectRetryDelay);
      m_connectRetryTimer.async_wait([this](const boost::system::error_code& ec){ reconnectTimerHandler(ec); });
      return;
   }

   // Connected
   notifyEventHandler();

   // Start listening on the port
   startRead();
}

void CAsyncTcpClient::startRead()
{
   // Start an asynchronous read and call readCompleted when it completes or fails 
   m_boostSocket.async_read_some(
      boost::asio::buffer(m_asyncReadBuffer.begin(), m_asyncReadBuffer.size()),
      [this](const boost::system::error_code& ec, std::size_t n){
         readCompleted(ec, n);
      });
}

void CAsyncTcpClient::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is fired when stop is required
      if (error == boost::asio::error::operation_aborted)
         return;     // Normal stop

      // Error ==> disconnecting
      YADOMS_LOG(error) << "Socket read error : " << error.message();
      disconnect();
      notifyEventHandler("asyncPort.tcp.operationAborted");
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

void CAsyncTcpClient::send(const CByteBuffer& buffer)
{
   sendBuffer(boost::asio::const_buffer(buffer.begin(), buffer.size()));
}

void CAsyncTcpClient::sendText(const std::string & content)
{
   sendBuffer(boost::asio::buffer(content));
}

void CAsyncTcpClient::sendBuffer(const boost::asio::const_buffer& buffer)
{
   try
   {
      m_boostSocket.write_some(buffer);
   }
   catch (boost::system::system_error& e)
   {
      // boost::asio::error::eof is the normal stop
      if (e.code() != boost::asio::error::eof)
      {
         YADOMS_LOG(error) << "Socket write text error : " << e.what();
         disconnect();
      }

      if(e.code() == boost::asio::error::eof)
         notifyEventHandler("asyncPort.tcp.connectionClosed");
      else
         notifyEventHandler("asyncPort.tcp.connectionError");

      throw CPortException((e.code() == boost::asio::error::eof) ? CPortException::kConnectionClosed : CPortException::kConnectionError, e.what());
   }
}

void CAsyncTcpClient::notifyEventHandler()
{
   if (m_connectStateEventHandler)
   {
      auto param = boost::make_shared<CAsyncPortConnectionNotification>();
      m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
   }
}

void CAsyncTcpClient::notifyEventHandler(const std::string & i18nErrorMessage)
{
   if (m_connectStateEventHandler)
   {
      auto param = boost::make_shared<CAsyncPortConnectionNotification>(i18nErrorMessage);
      m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
   }
}

} } // namespace shared::communication
