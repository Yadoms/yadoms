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

      typedef FT_STATUS(__stdcall *f_ftopen)(int deviceNumber, FT_HANDLE *handle);
      typedef FT_STATUS(__stdcall *f_ftclose)(FT_HANDLE handle);
      typedef FT_STATUS(__stdcall *f_ftsetBitMode)(FT_HANDLE handle, UCHAR ucMask, UCHAR ucEnabled);
      typedef FT_STATUS(__stdcall *f_ftgetBitMode)(FT_HANDLE handle, PUCHAR pucMode);
      typedef FT_STATUS(__stdcall *f_ftsetbaudRate)(FT_HANDLE handle, ULONG baudRate);
      typedef FT_STATUS(__stdcall *f_ftsetDataCharacteristics)(FT_HANDLE ftHandle, UCHAR uWordLength, UCHAR uStopBits, UCHAR uParity);
      typedef FT_STATUS(__stdcall *f_ftsetEventNotification)(FT_HANDLE ftHandle, DWORD dwEventMask, PVOID pvArg);      typedef FT_STATUS(__stdcall *f_ftGetStatus)(FT_HANDLE ftHandle, LPDWORD lpdwAmountInRxQueue, LPDWORD lpdwAmountInTxQueue, LPDWORD lpdwEventStatus);      typedef FT_STATUS(__stdcall *f_ftGetModemStatus)(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus);      typedef FT_STATUS(__stdcall *f_ftRead)(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpdwBytesReturned);      typedef FT_STATUS(__stdcall *f_ftCreateDeviceInfoList)(LPDWORD lpdwNumDevs);      typedef FT_STATUS(__stdcall *f_ftGetDeviceInfoList)(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);      typedef FT_STATUS(__stdcall *f_ftGetDeviceInfoDetail)(DWORD dwIndex, LPDWORD lpdwFlags,
                                                            LPDWORD lpdwType,
                                                            LPDWORD lpdwID, LPDWORD lpdwLocId,
                                                            PCHAR pcSerialNumber, PCHAR pcDescription,
                                                            FT_HANDLE *ftHandle);

      CFT2xxSerialPort::CFT2xxSerialPort(const std::string& port,
                                         const boost::asio::serial_port_base::baud_rate& baudrate,
                                         const boost::asio::serial_port_base::parity& parity,
                                         const boost::asio::serial_port_base::character_size& characterSize,
                                         const boost::asio::serial_port_base::stop_bits& stop_bits,
                                         const boost::asio::serial_port_base::flow_control& flowControl,
                                         const boost::posix_time::time_duration& connectRetryDelay,
                                         bool flushAtConnect)
         : //m_boostSerialPort(m_ioService),
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
           //m_connectRetryTimer(m_ioService),
           m_flushAtConnect(flushAtConnect),
           m_writeTimeout(boost::date_time::pos_infin),
           m_writeTimeouted(false),
         m_isConnected(false)
      {
         hGetProcIDDLL = LoadLibraryA("ftd2xx.dll");

         if (!hGetProcIDDLL)
         {
            YADOMS_LOG(error) << "Could not load the dynamic library";
            // TODO : throw an error
         }
         else
         {
            hEvent = CreateEventA(
               NULL,
               false, // auto-reset event
               false, // non-signalled state
               "");

            /* TODO : Complete the creation of the event
            if (ghWriteEvent == NULL)
            {
               printf("CreateEvent failed (%d)\n", GetLastError());
               return;
            }*/
         }
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
         if (!!m_asyncThread)
            return; // Already started

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

         SetEvent(hEvent);
         m_asyncThread->join();
         m_asyncThread.reset();
      }

      void CFT2xxSerialPort::receiverThread()
      {
         FT_STATUS	ftStatus;

         f_ftGetStatus FT_GetStatus = (f_ftGetStatus)GetProcAddress(hGetProcIDDLL, "FT_GetStatus");
         f_ftGetModemStatus FT_GetModemStatus = (f_ftGetModemStatus)GetProcAddress(hGetProcIDDLL, "FT_GetModemStatus");
         f_ftRead FT_Read = (f_ftRead)GetProcAddress(hGetProcIDDLL, "FT_Read");
         
         YADOMS_LOG(debug) << "Create receiverThread";

         while (true)
         {
            WaitForSingleObject(hEvent, INFINITE);

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
               YADOMS_LOG(debug) << "FT_Read failed";
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
                  // FT_Read Failed
                  YADOMS_LOG(debug) << "FT_Read failed";
                  break;
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
            FT_DEVICE_LIST_INFO_NODE *devInfo;
            YADOMS_LOG(debug) << "Open " << m_port << "...";

            f_ftopen  FT_Open = (f_ftopen)GetProcAddress(hGetProcIDDLL, "FT_Open");
            f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");
            f_ftsetDataCharacteristics  FT_SetDataCharacteristics = (f_ftsetDataCharacteristics)GetProcAddress(hGetProcIDDLL, "FT_SetDataCharacteristics");
            f_ftCreateDeviceInfoList FT_CreateDeviceInfoList = (f_ftCreateDeviceInfoList)GetProcAddress(hGetProcIDDLL, "FT_CreateDeviceInfoList");
            f_ftGetDeviceInfoList FT_GetDeviceInfoList = (f_ftGetDeviceInfoList)GetProcAddress(hGetProcIDDLL, "FT_GetDeviceInfoList");
            f_ftGetDeviceInfoDetail FT_GetDeviceInfoDetail = (f_ftGetDeviceInfoDetail)GetProcAddress(hGetProcIDDLL, "FT_GetDeviceInfoDetail");

            DWORD Flags;
            DWORD ID;
            DWORD Type;
            DWORD LocId;
            char SerialNumber[16];
            char Description[64];

            DWORD numDevs;

            // create the device information list
            ftStatus = FT_CreateDeviceInfoList(&numDevs);
            if (ftStatus == FT_OK) {
               YADOMS_LOG(information) << "Number of devices is " << numDevs;
            }
            else {
               YADOMS_LOG(error) << "FT_CreateDeviceInfoList failed";
            }

            if (numDevs > 0) {
               FT_HANDLE ftHandleTemp;
               // get information for device 0
               ftStatus = FT_GetDeviceInfoDetail(0, &Flags, &Type, &ID, &LocId, SerialNumber,
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

               // allocate storage for list based on numDevs
               devInfo = (FT_DEVICE_LIST_INFO_NODE*)malloc(sizeof(FT_DEVICE_LIST_INFO_NODE)*numDevs);
               // get the device information list
               ftStatus = FT_GetDeviceInfoList(devInfo, &numDevs);
               if (ftStatus == FT_OK) {
                  for (int i = 0; i < numDevs; i++) {
                     YADOMS_LOG(debug) <<"Dev :" << i;
                     YADOMS_LOG(debug) << " Flags=" << devInfo[i].Flags;
                     YADOMS_LOG(debug) << " Type=" << devInfo[i].Type;
                     YADOMS_LOG(debug) << " ID=" << devInfo[i].ID;
                     YADOMS_LOG(debug) << " LocId=" << devInfo[i].LocId;
                     YADOMS_LOG(debug) << " SerialNumber=" << devInfo[i].SerialNumber;
                     YADOMS_LOG(debug) << " Description=" << devInfo[i].Description;
                     YADOMS_LOG(debug) << " ftHandle=" << devInfo[i].ftHandle;
                  }
               }
            }

            ftStatus = FT_Open(0, &ftHandle);
            if (ftStatus != FT_OK) {
               // TODO : Send a exception - perhaps fail because the driver is not installed
               YADOMS_LOG(error) << "FT_Open failed";
            }

            ftStatus = FT_SetBaudRate(ftHandle, m_baudrate.value());
            if (ftStatus != FT_OK) {
               // TODO : Send a exception - perhaps fail because the driver is not installed
               YADOMS_LOG(error) << "FT_SetBaudRate failed";
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
               // TODO : Send a exception - perhaps fail because the driver is not installed
            }

            m_isConnected = true;
         }
         catch (boost::system::system_error& e)
         {
            notifyEventHandler("asyncPort.serial.failToOpen", {{ "port", m_port }});
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
            f_ftclose FT_Close = (f_ftclose)GetProcAddress(hGetProcIDDLL, "FT_Close");

            if (ftHandle != NULL) {
               FT_Close(ftHandle);
               ftHandle = NULL;
               m_isConnected = false;
            }
            YADOMS_LOG(debug) << " : m_boostSerialPort.close();";
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
      /*
      void CFT2xxSerialPort::reconnectTimerHandler(const boost::system::error_code& error)
      {
         if (error)
            throw exception::CException("Error code should be 0 here");

         tryConnect();
      }*/

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
         // Start an asynchronous read and call readCompleted when it completes or fails
         FT_STATUS	ftStatus;
         f_ftsetEventNotification  FT_SetEventNotification = (f_ftsetEventNotification)GetProcAddress(hGetProcIDDLL, "FT_SetEventNotification");
         DWORD EventMask = FT_EVENT_RXCHAR | FT_EVENT_MODEM_STATUS;

         ftStatus = FT_SetEventNotification(ftHandle, EventMask, hEvent);
         /*
         m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.begin(),
                                                               m_asyncReadBuffer.size()),
                                           boost::bind(&CFT2xxSerialPort::readCompleted,
                                                       this,
                                                       boost::asio::placeholders::error,
                                                       boost::asio::placeholders::bytes_transferred));
                                                       */
      }
      /*
      void CFT2xxSerialPort::readCompleted(const boost::system::error_code& error,
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
               notifyEventHandler("asyncPort.serial.failToCommunicateWithHardware", { {"message", error.message() }, { "code" , (boost::format("%1%") % error.value()).str() } });
            else
               notifyEventHandler("asyncPort.serial.error", { { "message", error.message() },{ "code" , (boost::format("%1%") % error.value()).str() } });
            return;
         }

         // Read OK
         CByteBuffer buffer(bytesTransferred);
         memcpy(buffer.begin(), m_asyncReadBuffer.begin(), bytesTransferred);

         if (!!m_receiveBufferHandler)
            m_receiveBufferHandler->push(buffer);

         // Restart read
         startRead();
      }*/

      void CFT2xxSerialPort::activateGPIO(const int GPIONumber)
      {
         unsigned char ucMask = 0xFF;
         unsigned char ucMode;
         FT_STATUS	ftStatus;

         f_ftsetBitMode FT_SetBitMode = (f_ftsetBitMode)GetProcAddress(hGetProcIDDLL, "FT_SetBitMode");
         f_ftgetBitMode FT_GetBitMode = (f_ftgetBitMode)GetProcAddress(hGetProcIDDLL, "FT_GetBitMode");

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