#pragma once
#include <shared/communication/IAsyncPort.h>
#include <shared/communication/IReceiveBufferHandler.h>
#include <shared/communication/AsyncPortConnectionNotification.h>
#include "../ftdi/ftd2xx.h"

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	This class manage a serial port
      //--------------------------------------------------------------
      class CFT2xxSerialPort : public IAsyncPort
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] port                 Serial port name
         /// \param[in] baudrate             Baudrate (in bauds)
         /// \param[in] parity               Parity (see boost::asio::serial_port_base::parity::type for values)
         /// \param[in] characterSize        Character size (from 5 to 8)
         /// \param[in] stop_bits            Nb of stop bits (see boost::asio::serial_port_base::stop_bits::type for values)
         /// \param[in] flowControl          Flow control (see boost::asio::serial_port_base::flow_control::type for values)
         /// \param[in] connectRetryDelay    Delay between 2 connection retries
         /// \param[in] flushAtConnect       If true (default), flush serial port buffers before listening on port
         //--------------------------------------------------------------
         explicit CFT2xxSerialPort(const std::string name,
                                   const boost::asio::serial_port_base::baud_rate& baudrate = boost::asio::serial_port_base::baud_rate(9600),
                                   const boost::asio::serial_port_base::parity& parity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none),
                                   const boost::asio::serial_port_base::character_size& characterSize = boost::asio::serial_port_base::character_size(8),
                                   const boost::asio::serial_port_base::stop_bits& stop_bits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one),
                                   const boost::asio::serial_port_base::flow_control& flowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none),
                                   const boost::posix_time::time_duration& connectRetryDelay = boost::posix_time::seconds(30),
                                   bool flushAtConnect = true);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CFT2xxSerialPort();

         // IAsyncPort Implementation
         void setReceiveBufferMaxSize(std::size_t size) override;
         void start() override;
         void stop() override;
         bool isConnected() const override;
         void subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId) override;
         void setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler) override;
         void send(const CByteBuffer& buffer) override;
         void sendText(const std::string & content) override;
         void flush() override;
         // [END] IAsyncPort Implementation

         //-----------------------------------------------------
         ///\brief                            activate a GPIO
         /// \param [in] GPIONumber             the GPIO number
         //-----------------------------------------------------
         void activateGPIO(const int GPIONumber);

         //-----------------------------------------------------
         ///\brief                            desactivate GPIOs
         //-----------------------------------------------------
         void desactivateGPIO();

         //-----------------------------------------------------
         ///\brief                            get a vector with port FTDI serial port number existing for the driver
         /// \return                          the vector with all port number
         //-----------------------------------------------------
         std::vector<int> getPortComNumber();

         //-----------------------------------------------------
         ///\brief                            set the port number
         /// \param [in] port                   the port number for the FTDI driver
         //-----------------------------------------------------
         void setPortNumber(int port);

         //-----------------------------------------------------
         ///\brief                            set the baud rate speed
         /// \param [in] baudrate              the new baudrate to applicate
         //-----------------------------------------------------
         void setBaudRate(const boost::asio::serial_port_base::baud_rate& baudrate);
      protected:
         //--------------------------------------------------------------
         /// \brief	Establish the connection
         /// \return true if connected, false else
         //--------------------------------------------------------------
         virtual bool connect();

         //--------------------------------------------------------------
         /// \brief	Close the connection
         //--------------------------------------------------------------
         virtual void disconnect();

         //--------------------------------------------------------------
         /// \brief	Try to connect asynchronously
         //--------------------------------------------------------------
         void tryConnect();

         //--------------------------------------------------------------
         /// \brief	Wait for something to read on the port
         //--------------------------------------------------------------
         void startRead();

         //--------------------------------------------------------------
         /// \brief	                     Notify the event handler for connection event : SUCCESS
         //--------------------------------------------------------------
         void notifyEventHandler() const;

         //--------------------------------------------------------------
         /// \brief	                           Notify the event handler for connection event : ERROR
         /// \param[in] i18nErrorMessage        The i18n string representing the error : "serialport.failToOpen" -> "Impossible d'ouvrir le port série"
         //--------------------------------------------------------------
         void notifyEventHandler(const std::string & i18nErrorMessage) const;

         //--------------------------------------------------------------
         /// \brief	                           Notify the event handler for connection event : ERROR
         /// \param[in] i18nErrorMessage        The i18n string representing the error : "serialport.failToOpen" -> "Impossible d'ouvrir le port série {{failPort}}"
         /// \param[in] m_i18nMessageParameters The i18n string parameters : ie: ["failPort", "COM1"]
         //--------------------------------------------------------------
         void notifyEventHandler(const std::string & i18nErrorMessage, const std::map<std::string, std::string> & m_i18nMessageParameters) const;

      private:
         //--------------------------------------------------------------
         /// \brief	boost:asio service
         //--------------------------------------------------------------
         void receiverThread();

         //--------------------------------------------------------------
         /// \brief	Name of the serial port
         //--------------------------------------------------------------
         std::string m_name;

         //--------------------------------------------------------------
         /// \brief	Serial port configuration
         //--------------------------------------------------------------
         boost::asio::serial_port_base::baud_rate m_baudrate;
         boost::asio::serial_port_base::parity m_parity;
         boost::asio::serial_port_base::character_size m_characterSize;
         boost::asio::serial_port_base::stop_bits m_stop_bits;
         boost::asio::serial_port_base::flow_control m_flowControl;

         //--------------------------------------------------------------
         /// \brief	Read buffer for asynchronous operations
         //--------------------------------------------------------------
         CByteBuffer m_asyncReadBuffer;

         //--------------------------------------------------------------
         /// \brief	The event handler to notify for connection events   
         //--------------------------------------------------------------
         event::CEventHandler* m_connectStateEventHandler;

         //--------------------------------------------------------------
         /// \brief	The event id to notify for connection events  
         //--------------------------------------------------------------
         int m_connectStateEventId;

         //--------------------------------------------------------------
         /// \brief	The receive buffer handler
         //--------------------------------------------------------------
         boost::shared_ptr<IReceiveBufferHandler> m_receiveBufferHandler;

         //--------------------------------------------------------------
         /// \brief	Try to reconnect timer delay
         //--------------------------------------------------------------
         const boost::posix_time::time_duration m_connectRetryDelay;

         //--------------------------------------------------------------
         /// \brief	Thread for asynchronous operations
         //--------------------------------------------------------------
         boost::shared_ptr<boost::thread> m_asyncThread;

         //--------------------------------------------------------------
         /// \brief	Flush serial port buffers before listening on port (just after connection)
         //--------------------------------------------------------------
         bool m_flushAtConnect;

         FT_HANDLE m_ftHandle = NULL;
         HINSTANCE m_hGetProcIDDLL;
         HANDLE m_hEvent;

         static const unsigned char m_mask_port1;
         static const unsigned char m_mask_port2;
         static const unsigned char m_mask_no_port;
         static const unsigned char m_enableBitBang;

         bool m_isConnected;
         std::vector<int> m_SerialPortComNumber;
         int m_port;
      };
   }
} // namespace shared::communication