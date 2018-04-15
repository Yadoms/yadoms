#include "stdafx.h"
#include "FT2xxSerialPort.h"
#include <shared/Log.h>
#include <shared/Peripherals.h>
#include <shared/communication/PortException.hpp>
#include <shared/communication/Buffer.hpp>

namespace shared
{
   namespace communication
   {
      const unsigned char CFT2xxSerialPort::m_mask_port1 = 0x11;
      const unsigned char CFT2xxSerialPort::m_mask_port2 = 0x22;
      const unsigned char CFT2xxSerialPort::m_mask_no_port = 0x00;
      const unsigned char CFT2xxSerialPort::m_enableBitBang = 0x20;

      CFT2xxSerialPort::CFT2xxSerialPort(const boost::asio::serial_port_base::baud_rate& baudrate,
                                         const boost::asio::serial_port_base::parity& parity,
                                         const boost::asio::serial_port_base::character_size& characterSize,
                                         const boost::asio::serial_port_base::stop_bits& stop_bits,
                                         const boost::asio::serial_port_base::flow_control& flowControl,
                                         const boost::posix_time::time_duration& connectRetryDelay,
                                         bool flushAtConnect)
         : m_baudrate(baudrate),
           m_parity(parity),
           m_characterSize(characterSize),
           m_stop_bits(stop_bits),
           m_flowControl(flowControl),
           m_asyncReadBuffer(512),
           m_connectStateEventHandler(nullptr),
           m_connectStateEventId(event::kNoEvent),
           m_connectRetryDelay(connectRetryDelay),
           m_flushAtConnect(flushAtConnect),
           m_isConnected(false),
           m_port(0),
           mutex(PTHREAD_MUTEX_INITIALIZER)
      {
      }

      void CFT2xxSerialPort::setPortNumber(int port)
      {
         m_port = port;
      }

      std::vector<int> CFT2xxSerialPort::getPortComNumber()
      {
         FT_STATUS	ftStatus;
         DWORD numDevs;
         YADOMS_LOG(information) << "Scan all FT2X ports ...";

         // create the device information list
         ftStatus = FT_CreateDeviceInfoList(&numDevs);
         if (ftStatus == FT_OK) {
            YADOMS_LOG(information) << "Number of devices is " << numDevs;
         }
         else {
            std::string message = "Could not execute FT_CreateDeviceInfoList function";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }

         for (unsigned char counter = 0; counter < numDevs; ++counter)
         {
            ftStatus = FT_Open(counter, &ftHandle);
            if (ftStatus != FT_OK) {
               std::string message = "Could not execute FT_Open function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            LONG lplComPortNumber = 0;
            ftStatus = FT_GetComPortNumber(ftHandle, &lplComPortNumber);
            if (ftStatus != FT_OK) {
               std::string message = "Could not execute FT_GetComPortNumber function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            YADOMS_LOG(information) << "Port Com number : " << (int)lplComPortNumber;
            m_SerialPortComNumber.push_back(lplComPortNumber);

            if (ftHandle != NULL) {
               FT_Close(ftHandle);
               ftHandle = NULL;
               m_isConnected = false;
            }
         }

         return m_SerialPortComNumber;
      }

      CFT2xxSerialPort::~CFT2xxSerialPort()
      {
         CFT2xxSerialPort::stop();
      }

      void CFT2xxSerialPort::setReceiveBufferMaxSize(std::size_t size)
      {
         m_asyncReadBuffer.resize(size);
      }

      void CFT2xxSerialPort::start()
      {
         YADOMS_LOG(information) << "CFT2xxSerialPort::start()";
         if (!!m_asyncThread)
            return; // Already started

         YADOMS_LOG(information) << "tryConnect()";
         // Try to connect
         tryConnect();

         // instantiate the receiver thread
         m_asyncThread = boost::make_shared<boost::thread>(boost::bind(&CFT2xxSerialPort::receiverThread, this));
      }

      void CFT2xxSerialPort::stop()
      {
         if (!m_asyncThread)
            return; // Already stopped

         disconnect();

         //SetEvent(hEvent);
         m_asyncThread->join();
         m_asyncThread.reset();
      }

      void CFT2xxSerialPort::receiverThread()
      {
         FT_STATUS	ftStatus;
         YADOMS_LOG(debug) << "Create receiverThread";

         while (true)
         {
            pthread_mutex_lock(&mutex);
            pthread_mutex_unlock(&mutex);

            DWORD EventDWord;
            DWORD RxBytes;
            DWORD TxBytes;
            DWORD Status;
            DWORD BytesReceived;
            char RxBuffer[256];

            FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
            if (EventDWord & FT_EVENT_MODEM_STATUS) {
               // modem status event detected, so get current modem status
               FT_GetModemStatus(ftHandle, &Status);
               YADOMS_LOG(debug) << "Break thread ... modem status event detected";
               break;
            }
            if (RxBytes > 0) {
               ftStatus = FT_Read(ftHandle, RxBuffer, RxBytes, &BytesReceived);

               // Read OK
               CByteBuffer buffer(BytesReceived);
               memcpy(buffer.begin(), RxBuffer, BytesReceived);

               if (ftStatus == FT_OK) {
                  // FT_Read OK
                  if (!!m_receiveBufferHandler && BytesReceived>0)
                     m_receiveBufferHandler->push(buffer);
               }
               else {
                  std::string message = "Could not execute FT_Read function";
                  YADOMS_LOG(error) << message;
                  throw shared::exception::CException(message);
               }
            }
         }

         YADOMS_LOG(debug) << "Finish receiverThread";
      }

      bool CFT2xxSerialPort::connect()
      {
         // Open the port
         try
         {
            FT_STATUS	ftStatus;
            DWORD Flags;
            DWORD ID;
            DWORD Type;
            DWORD LocId;
            char SerialNumber[16];
            char Description[64];

            DWORD numDevs;

            YADOMS_LOG(information) << "CFT2xxSerialPort::connect()";

            // create the device information list
            ftStatus = FT_CreateDeviceInfoList(&numDevs);
            if (ftStatus == FT_OK) {
               YADOMS_LOG(information) << "Number of devices is " << numDevs;
            }
            else {
               std::string message = "Could not execute FT_CreateDeviceInfoList function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            if (numDevs > 0) {
               FT_HANDLE ftHandleTemp;
               // get information for device 0
               ftStatus = FT_GetDeviceInfoDetail(m_port, &Flags, &Type, &ID, &LocId, SerialNumber,
                                                 Description, &ftHandleTemp);
               if (ftStatus == FT_OK) {
                  YADOMS_LOG(debug) << "Dev 0:";
                  YADOMS_LOG(debug) << " Flags=" << Flags;
                  YADOMS_LOG(debug) << " Type=" << Type;
                  YADOMS_LOG(debug) << " ID=" << ID;
                  YADOMS_LOG(debug) << " LocId=" << LocId;
                  YADOMS_LOG(debug) << " SerialNumber=" << SerialNumber;
                  YADOMS_LOG(debug) << " Description=" << Description;
                  YADOMS_LOG(debug) << " ftHandle=" << ftHandleTemp;
               }
            }

            ftStatus = FT_Open(0, &ftHandle);
            if (ftStatus != FT_OK) {
               std::string message = "Could not execute FT_Open function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            ftStatus = FT_SetBaudRate(ftHandle, m_baudrate.value());
            if (ftStatus != FT_OK) {
               std::string message = "Could not execute FT_SetBaudRate function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            UCHAR stop_bits;
            UCHAR parity;

            if (m_stop_bits.value() == boost::asio::serial_port_base::stop_bits::one) 
               stop_bits = FT_STOP_BITS_1;
            else if (m_stop_bits.value() == boost::asio::serial_port_base::stop_bits::two)
               stop_bits = FT_STOP_BITS_2;
            else
               stop_bits = FT_STOP_BITS_1;

            if (m_parity.value() == boost::asio::serial_port_base::parity::even)
               parity = FT_PARITY_EVEN;
            else if (m_parity.value() == boost::asio::serial_port_base::parity::odd)
               parity = FT_PARITY_ODD;
            else if (m_parity.value() == boost::asio::serial_port_base::parity::none)
               parity = FT_PARITY_NONE;
            else 
               parity = FT_PARITY_NONE;

            ftStatus = FT_SetDataCharacteristics(ftHandle, m_characterSize.value(), stop_bits, parity);
            if (ftStatus != FT_OK) {
               std::string message = "Could not execute FT_SetDataCharacteristics function";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            m_isConnected = true;
         }
         catch (boost::system::system_error& e)
         {
            //notifyEventHandler("asyncPort.serial.failToOpen", {{ "port", m_port }});
            YADOMS_LOG(error) << " : Failed to open serial port : " << e.what();
            return false;
         }
         return true;
      }

      void CFT2xxSerialPort::disconnect()
      {
         if (!isConnected())
            return;

         // Close the port
         try
         {
            if (ftHandle != NULL) {
               FT_Close(ftHandle);
               ftHandle = NULL;
               m_isConnected = false;
            }
         }
         catch (boost::system::system_error& e)
         {
            YADOMS_LOG(error) << " : Failed to close serial port " << e.what();
         }
      }

      bool CFT2xxSerialPort::isConnected() const
      {
         return m_isConnected;
      }

      void CFT2xxSerialPort::subscribeForConnectionEvents(event::CEventHandler& forEventHandler,
                                                          int forId)
      {
         m_connectStateEventHandler = &forEventHandler;
         m_connectStateEventId = forId;
      }

      void CFT2xxSerialPort::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
      {
         m_receiveBufferHandler = handler;
      }

      void CFT2xxSerialPort::flush()
      {
         // Hardware flush
         //CPeripherals::flushSerialPort(m_boostSerialPort);

         // Clear the receive buffer
         if (!!m_receiveBufferHandler)
            m_receiveBufferHandler->flush();
      }

      void CFT2xxSerialPort::tryConnect()
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

               //TODO : Mettre en place la relance
               //m_connectRetryTimer.expires_from_now(m_connectRetryDelay);
               //m_connectRetryTimer.async_wait(boost::bind(&CFT2xxSerialPort::reconnectTimerHandler, this, boost::asio::placeholders::error));
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

      void CFT2xxSerialPort::startRead()
      {
         YADOMS_LOG(debug) << "CFT2xxSerialPort::startRead()";

         // Start an asynchronous read and call readCompleted when it completes or fails
         FT_STATUS	ftStatus;
         DWORD EventMask = FT_EVENT_RXCHAR | FT_EVENT_MODEM_STATUS;

         pthread_mutex_init(&mutex, NULL);
         ftStatus = FT_SetEventNotification(ftHandle, EventMask, &mutex);

         if (ftStatus != FT_OK) {
            std::string message = "Could not execute FT_SetEventNotification function";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
      }

      void CFT2xxSerialPort::activateGPIO(const int GPIONumber)
      {
         unsigned char ucMask = 0xFF;
         unsigned char ucMode;
         FT_STATUS	ftStatus;

         switch (GPIONumber)
         {
         case 1:
            ucMask = m_mask_port1;
            break;
         case 2:
            ucMask = m_mask_port2;
            break;
         default:
            ucMask = m_mask_no_port;
         }

         ucMode = m_enableBitBang;

         YADOMS_LOG(debug) << "ftHandle " << ftHandle;

         ftStatus = FT_SetBitMode(ftHandle, ucMask, ucMode);

         if (ftStatus != FT_OK)
            YADOMS_LOG(error) << "Failed to set bit mode for port: " << GPIONumber;
         else
            YADOMS_LOG(debug) << "Set bit mode for port: " << GPIONumber;

         ftStatus = FT_GetBitMode(ftHandle, &ucMode);
         if (ftStatus != FT_OK) {
            YADOMS_LOG(error) << "Failed to get bit mode";
         }       
      }

      void CFT2xxSerialPort::desactivateGPIO()
      {
         // No GPIO activate => No access
         activateGPIO(0);
      }

      void CFT2xxSerialPort::send(const CByteBuffer& buffer)
      {
         // Not used here 
      }

      void CFT2xxSerialPort::sendText(const std::string& content)
      {
         // Not used here
      }

      void CFT2xxSerialPort::notifyEventHandler() const
      {
         if (m_connectStateEventHandler)
         {
            auto param = boost::make_shared<CAsyncPortConnectionNotification>();
            m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
         }
      }

      void CFT2xxSerialPort::notifyEventHandler(const std::string & i18nErrorMessage) const
      {
         if (m_connectStateEventHandler)
         {
            auto param = boost::make_shared<CAsyncPortConnectionNotification>(i18nErrorMessage);
            m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
         }
      }

      void CFT2xxSerialPort::notifyEventHandler(const std::string & i18nErrorMessage, const std::map<std::string, std::string> & i18nMessageParameters) const
      {
         if (m_connectStateEventHandler)
         {
            auto param = boost::make_shared<CAsyncPortConnectionNotification>(i18nErrorMessage, i18nMessageParameters);
            m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
         }
      }
   }
} // namespace shared::communication
