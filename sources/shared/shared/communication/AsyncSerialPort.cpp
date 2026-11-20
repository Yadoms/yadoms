#include "stdafx.h"
#include "AsyncSerialPort.h"
#include <future>
#include <shared/Log.h>
#include <shared/SerialPortHelper.h>
#include "PortException.hpp"
#include "Buffer.hpp"

using namespace shared::communication;

CAsyncSerialPort::CAsyncSerialPort(const std::string& port,
								   const boost::asio::serial_port_base::baud_rate& baudrate,
								   const boost::asio::serial_port_base::parity& parity,
								   const boost::asio::serial_port_base::character_size& characterSize,
								   const boost::asio::serial_port_base::stop_bits& stop_bits,
								   const boost::asio::serial_port_base::flow_control& flowControl,
								   const std::chrono::milliseconds& connectRetryDelay,
								   bool flushAtConnect)
	: m_boostSerialPort(m_io),
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
	m_connectRetryTimer(m_io),
	m_flushAtConnect(flushAtConnect),
	m_writeTimeout(std::chrono::milliseconds::max()),
	m_writeTimeouted(false),
	m_stopping(false)
{
}

CAsyncSerialPort::~CAsyncSerialPort()
{
	CAsyncSerialPort::stop();
}

void CAsyncSerialPort::setWriteTimeout(const std::chrono::milliseconds& writeTimeout)
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

	m_io.restart();
	m_workGuard.emplace(m_io.get_executor());

	// Try to connect
	tryConnect();
	m_asyncThread = boost::make_shared<std::thread>([this] { m_io.run(); });
}

void CAsyncSerialPort::stop()
{
	if (!m_asyncThread)
		return; // Already stopped

	// Serial port object is not thread-safe : close it from the asynchronous thread
	std::promise<void> disconnected;
	auto disconnectedFuture = disconnected.get_future();
	boost::asio::post(m_io, [this, &disconnected]
	{
		m_stopping = true;
		m_connectRetryTimer.cancel();
		disconnect();
		disconnected.set_value();
	});

	if (disconnectedFuture.wait_for(std::chrono::seconds(5)) == std::future_status::timeout)
		YADOMS_LOG(error) << " : Timeout closing serial port " << m_port << ", serial driver seems to be blocked";

	m_workGuard.reset();
	m_io.stop();
	m_asyncThread->join();
	m_asyncThread.reset();
	m_stopping = false;
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
		notifyEventHandler("asyncPort.serial.failToOpen", { {"port", m_port} });
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

	// Close the port, even if cancel fails (the port must not stay open with an invalid handle)
	boost::system::error_code ec;
	m_boostSerialPort.cancel(ec);
	if (ec)
		YADOMS_LOG(warning) << " : Failed to cancel serial port operations : " << ec.message();

	m_boostSerialPort.close(ec);
	if (ec)
		YADOMS_LOG(error) << " : Failed to close serial port : " << ec.message();
}

void CAsyncSerialPort::asyncDisconnect()
{
	boost::asio::post(m_io, [this] { disconnect(); });
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
	if (error == boost::asio::error::operation_aborted || m_stopping)
		return;

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
			m_connectRetryTimer.expires_after(m_connectRetryDelay);
			m_connectRetryTimer.async_wait([this](const boost::system::error_code& ec) {
				reconnectTimerHandler(ec);
			});
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
	if (m_stopping || !isConnected())
		return;

	// Start an asynchronous read and call readCompleted when it completes or fails 
	m_boostSerialPort.async_read_some(
		boost::asio::buffer(m_asyncReadBuffer.begin(), m_asyncReadBuffer.size()),
		[this](const boost::system::error_code& ec, std::size_t n) {
		readCompleted(ec, n);
	});
}

void CAsyncSerialPort::readCompleted(const boost::system::error_code& error,
									 std::size_t bytesTransferred)
{
	if (m_stopping)
		return;

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
			notifyEventHandler("asyncPort.serial.failToCommunicateWithHardware",
							   {
									{"message", error.message()},
									{"code", std::to_string(error.value())}
							   });
		else
			notifyEventHandler("asyncPort.serial.error",
							   {
									{"message", error.message()},
									{"code", std::to_string(error.value())}
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
	sendBuffer(boost::asio::const_buffer(buffer.begin(), buffer.size()));
}

void CAsyncSerialPort::sendText(const std::string& content)
{
	sendBuffer(boost::asio::buffer(content));
}

void CAsyncSerialPort::sendBuffer(const boost::asio::const_buffer& buffer)
{
	try
	{
		if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::none
			|| m_writeTimeout == std::chrono::milliseconds::max())
		{
			m_boostSerialPort.write_some(boost::asio::buffer(buffer));
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
			m_boostSerialPort.async_write_some(buffer,
											   [&evtHandler](const boost::system::error_code& ec,
															 std::size_t bytes_transferred)
			{
				evtHandler.postEvent(kSendFinished, ec);
			});

			switch (evtHandler.waitForEvents(boost::posix_time::milliseconds(m_writeTimeout.count())))
			{
			case event::kTimeout:
			{
				m_writeTimeouted = true;
				auto rc = boost::system::error_code(boost::system::errc::timed_out,
													boost::asio::error::get_misc_category());
				// Stop async operation
				m_boostSerialPort.cancel(rc);
				evtHandler.waitForEvents(boost::posix_time::milliseconds(m_writeTimeout.count()));

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
			asyncDisconnect();
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


