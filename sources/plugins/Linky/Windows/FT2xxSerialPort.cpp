#include "stdafx.h"
#include "FT2xxSerialPort.h"
#include <shared/Log.h>
#include <shared/Peripherals.h>
#include <shared/communication/PortException.hpp>
#include <shared/communication/Buffer.hpp>
#include <shared/exception/Exception.hpp>

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
      typedef FT_STATUS(__stdcall *f_ftsetEventNotification)(FT_HANDLE ftHandle, DWORD dwEventMask, PVOID pvArg);
      typedef FT_STATUS(__stdcall *f_ftGetStatus)(FT_HANDLE ftHandle, LPDWORD lpdwAmountInRxQueue, LPDWORD lpdwAmountInTxQueue, LPDWORD lpdwEventStatus);
      typedef FT_STATUS(__stdcall *f_ftGetModemStatus)(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus);
      typedef FT_STATUS(__stdcall *f_ftRead)(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead, LPDWORD lpdwBytesReturned);
      typedef FT_STATUS(__stdcall *f_ftCreateDeviceInfoList)(LPDWORD lpdwNumDevs);
      typedef FT_STATUS(__stdcall *f_ftGetDeviceInfoList)(FT_DEVICE_LIST_INFO_NODE *pDest, LPDWORD lpdwNumDevs);
      typedef FT_STATUS(__stdcall *f_ftGetComPortNumber)(FT_HANDLE ftHandle, LPLONG lplComPortNumber);
      typedef FT_STATUS(__stdcall *f_ftGetDriverVersion)(FT_HANDLE ftHandle, LPDWORD lpdwDriverVersion);
      typedef FT_STATUS(__stdcall *f_ftGetLibraryVersion)(LPDWORD lpdwDLLVersion);
      typedef FT_STATUS(__stdcall *f_ftSetFlowControl)(FT_HANDLE ftHandle, USHORT usFlowControl, UCHAR uXon, UCHAR uXoff);

      typedef FT_STATUS(__stdcall *f_ftGetDeviceInfoDetail)(DWORD dwIndex, LPDWORD lpdwFlags,
                                                            LPDWORD lpdwType,
                                                            LPDWORD lpdwID, LPDWORD lpdwLocId,
                                                            PCHAR pcSerialNumber, PCHAR pcDescription,
                                                            FT_HANDLE *ftHandle);



      CFT2xxSerialPort::CFT2xxSerialPort(const std::string name,
                                         const boost::asio::serial_port_base::baud_rate& baudrate,
                                         const boost::asio::serial_port_base::parity& parity,
                                         const boost::asio::serial_port_base::character_size& characterSize,
                                         const boost::asio::serial_port_base::stop_bits& stop_bits,
                                         const boost::asio::serial_port_base::flow_control& flowControl,
                                         const boost::posix_time::time_duration& connectRetryDelay,
                                         bool flushAtConnect) :
         m_name(name),
         m_baudrate(baudrate),
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
         m_port(0)
      {
         hGetProcIDDLL = LoadLibraryA("ftd2xx.dll");

         if (!hGetProcIDDLL)
         {
            std::string message = "Could not load the dynamic library";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
         else
         {
            hEvent = CreateEventA(
               NULL,
               false, // auto-reset event
               false, // non-signalled state
               "");

            if (hEvent == NULL)
            {
               std::string message = "Create event failed";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }
         }
      }

      void CFT2xxSerialPort::setPortNumber(int port)
      {
         m_port = port;
      }

      std::vector<int> CFT2xxSerialPort::getPortComNumber()
      {
         FT_STATUS	ftStatus;
         DWORD numDevs;
         f_ftopen  FT_Open = (f_ftopen)GetProcAddress(hGetProcIDDLL, "FT_Open");
         f_ftCreateDeviceInfoList FT_CreateDeviceInfoList = (f_ftCreateDeviceInfoList)GetProcAddress(hGetProcIDDLL, "FT_CreateDeviceInfoList");
         f_ftclose FT_Close = (f_ftclose)GetProcAddress(hGetProcIDDLL, "FT_Close");
         f_ftGetComPortNumber FT_GetComPortNumber = (f_ftGetComPortNumber)GetProcAddress(hGetProcIDDLL, "FT_GetComPortNumber");

         YADOMS_LOG(information) << "Scan all FT2X ports ...";

         // create the device information list
         ftStatus = FT_CreateDeviceInfoList(&numDevs);
         if (ftStatus == FT_OK) {
            YADOMS_LOG(information) << "Number of devices is " << numDevs;
         }
         else {
            YADOMS_LOG(error) << "FT_CreateDeviceInfoList failed";
         }

         for (unsigned char counter = 0; counter < numDevs; ++counter)
         {
            ftStatus = FT_Open(counter, &ftHandle);
            if (ftStatus != FT_OK) {
               std::string message = "Fail to open the serial port with FTDI driver";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            LONG lplComPortNumber = 0;
            ftStatus = FT_GetComPortNumber(ftHandle, &lplComPortNumber);
            if (ftStatus != FT_OK) {
               std::string message = "Fail to get the serial port number";
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
         YADOMS_LOG(debug) << "CFT2xxSerialPort::~CFT2xxSerialPort()";
         // We desactivate running GPIOs if activated
         desactivateGPIO();
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
         YADOMS_LOG(debug) << "CFT2xxSerialPort::stop()";
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
            char RxBuffer[2048] = { 0 };

            if (!m_isConnected) break;

            FT_GetStatus(ftHandle, &RxBytes, &TxBytes, &EventDWord);
            if (EventDWord & FT_EVENT_MODEM_STATUS) {
               // modem status event detected, so get current modem status
               FT_GetModemStatus(ftHandle, &Status);
               YADOMS_LOG(debug) << "Modem status event detected => exit";
               break;
            }
            if (RxBytes > 0) {
               ftStatus = FT_Read(ftHandle, RxBuffer, RxBytes, &BytesReceived);

               // Read OK
               CByteBuffer buffer(BytesReceived);
               memcpy(buffer.begin(), RxBuffer, BytesReceived);

               if (ftStatus == FT_OK) {
                  // FT_Read OK

                  if (!!m_receiveBufferHandler && BytesReceived > 0)
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
            f_ftopen  FT_Open = (f_ftopen)GetProcAddress(hGetProcIDDLL, "FT_Open");
            f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");
            f_ftsetDataCharacteristics  FT_SetDataCharacteristics = (f_ftsetDataCharacteristics)GetProcAddress(hGetProcIDDLL, "FT_SetDataCharacteristics");
            f_ftCreateDeviceInfoList FT_CreateDeviceInfoList = (f_ftCreateDeviceInfoList)GetProcAddress(hGetProcIDDLL, "FT_CreateDeviceInfoList");
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
            else
            {
               shared::exception::CException("No FTDI serial port detected");
            }

            ftStatus = FT_Open(m_port, &ftHandle);
            if (ftStatus != FT_OK) {
               std::string message = "Fail to open the serial port with the FTDI driver";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            DWORD dwDriverVersion;
            DWORD dwLibraryVersion;
            f_ftGetDriverVersion  FT_GetDriverVersion = (f_ftGetDriverVersion)GetProcAddress(hGetProcIDDLL, "FT_GetDriverVersion");
            f_ftGetLibraryVersion  FT_GetLibraryVersion = (f_ftGetLibraryVersion)GetProcAddress(hGetProcIDDLL, "FT_GetLibraryVersion");
            f_ftSetFlowControl  FT_SetFlowControl = (f_ftSetFlowControl)GetProcAddress(hGetProcIDDLL, "FT_SetFlowControl");
            

            ftStatus = FT_GetDriverVersion(ftHandle, &dwDriverVersion);

            if (ftStatus != FT_OK) {
               std::string message = "Could not get the driver version";
               throw shared::exception::CException(message);
            }
            else
            {
               //convert int to hex string
               std::stringstream stream;
               stream << std::hex << ((dwDriverVersion & 0xFF0000)>>16) << "."
                  << std::hex << ((dwDriverVersion & 0x00FF00)>>8) << "."
                  << std::hex << (dwDriverVersion & 0x0000FF);
               auto hexNumber(stream.str());
               YADOMS_LOG(information) << "FTDI driver version : " << hexNumber;
            }

            FT_GetLibraryVersion(&dwLibraryVersion);
            if (ftStatus != FT_OK) {
               std::string message = "Could not get the library version";
               throw shared::exception::CException(message);
            }
            else
            {
               //convert int to hex string
               std::stringstream stream;
               stream << std::hex << ((dwLibraryVersion & 0xFF0000)>>16) << "."
                  << std::hex << ((dwLibraryVersion & 0x00FF00)>>8) << "."
                  << std::hex << (dwLibraryVersion & 0x0000FF);
               auto hexNumber(stream.str());
               YADOMS_LOG(information) << "FTDI library version : " << hexNumber;
            }

            ftStatus = FT_SetBaudRate(ftHandle, m_baudrate.value());
            if (ftStatus != FT_OK) {
               std::string message = "Fail to set the baudrate";
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

            ftStatus = FT_SetDataCharacteristics(ftHandle, (UCHAR)m_characterSize.value(), stop_bits, parity);
            if (ftStatus != FT_OK) {
               std::string message = "Fail to set Data format";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            UCHAR flowControl;

            if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::hardware)
               flowControl = FT_FLOW_XON_XOFF;
            else if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::none)
               flowControl = FT_FLOW_NONE;
            else
               flowControl = FT_FLOW_NONE;

            ftStatus = FT_SetFlowControl(ftHandle, flowControl, 0x00, 0x00);
            if (ftStatus != FT_OK) {
               std::string message = "Fail to set Flow Control";
               YADOMS_LOG(error) << message;
               throw shared::exception::CException(message);
            }

            m_isConnected = true;
         }
         catch (boost::system::system_error& e)
         {
            notifyEventHandler("asyncPort.serial.failToOpen", { { "port", m_name } });
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
               m_isConnected = false;
               FT_Close(ftHandle);
               ftHandle = NULL;
               YADOMS_LOG(debug) << "Close the FTDI serial port";
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
               SetEvent(hEvent);
               m_asyncThread->join();
               m_asyncThread.reset();
               notifyEventHandler("asyncPort.serial.failToOpen", { { "port", m_name } });
               tryConnect();
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

         if (ftStatus != FT_OK)
         {
            std::string message = "Fail to set event notification";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
      }

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

      void CFT2xxSerialPort::setBaudRate(const boost::asio::serial_port_base::baud_rate& baudrate = boost::asio::serial_port_base::baud_rate(9600))
      {
         FT_STATUS	ftStatus;
         f_ftsetbaudRate FT_SetBaudRate = (f_ftsetbaudRate)GetProcAddress(hGetProcIDDLL, "FT_SetBaudRate");
         m_baudrate = baudrate;

         YADOMS_LOG(information) << m_baudrate.value();

         ftStatus = FT_SetBaudRate(ftHandle, m_baudrate.value());
         if (ftStatus != FT_OK) {
            std::string message = "Fail to set the baudrate";
            YADOMS_LOG(error) << message;
            throw shared::exception::CException(message);
         }
      }
   }
} // namespace shared::communication