#include "stdafx.h"
#include "FT2xxSerialPort.h"
#include <shared/Log.h>
#include <shared/communication/Buffer.hpp>
#include <shared/communication/AsyncPortConnectionNotification.h>
#include <shared/exception/Exception.hpp>


const unsigned char CFT2xxSerialPort::MaskPort1 = 0x11;
const unsigned char CFT2xxSerialPort::MaskPort2 = 0x22;
const unsigned char CFT2xxSerialPort::MaskNoPort = 0x00;
const unsigned char CFT2xxSerialPort::EnableBitBang = 0x20;

typedef FT_STATUS (__stdcall *f_ftopen)(int deviceNumber, FT_HANDLE* handle);
typedef FT_STATUS (__stdcall *f_ftclose)(FT_HANDLE handle);
typedef FT_STATUS (__stdcall *f_ftsetBitMode)(FT_HANDLE handle, UCHAR ucMask, UCHAR ucEnabled);
typedef FT_STATUS (__stdcall *f_ftgetBitMode)(FT_HANDLE handle, PUCHAR pucMode);
typedef FT_STATUS (__stdcall *f_ftsetbaudRate)(FT_HANDLE handle, ULONG baudRate);
typedef FT_STATUS (__stdcall *f_ftsetDataCharacteristics)(FT_HANDLE ftHandle, UCHAR uWordLength, UCHAR uStopBits,
                                                          UCHAR uParity);
typedef FT_STATUS (__stdcall *f_ftsetEventNotification)(FT_HANDLE ftHandle, DWORD dwEventMask, PVOID pvArg);
typedef FT_STATUS (__stdcall *f_ftGetStatus)(FT_HANDLE ftHandle, LPDWORD lpdwAmountInRxQueue,
                                             LPDWORD lpdwAmountInTxQueue, LPDWORD lpdwEventStatus);
typedef FT_STATUS (__stdcall *f_ftGetModemStatus)(FT_HANDLE ftHandle, LPDWORD lpdwModemStatus);
typedef FT_STATUS (__stdcall *f_ftRead)(FT_HANDLE ftHandle, LPVOID lpBuffer, DWORD dwBytesToRead,
                                        LPDWORD lpdwBytesReturned);
typedef FT_STATUS (__stdcall *f_ftCreateDeviceInfoList)(LPDWORD lpdwNumDevs);
typedef FT_STATUS (__stdcall *f_ftGetDeviceInfoList)(FT_DEVICE_LIST_INFO_NODE* pDest, LPDWORD lpdwNumDevs);
typedef FT_STATUS (__stdcall *f_ftGetComPortNumber)(FT_HANDLE ftHandle, LPLONG lplComPortNumber);
typedef FT_STATUS (__stdcall *f_ftGetDriverVersion)(FT_HANDLE ftHandle, LPDWORD lpdwDriverVersion);
typedef FT_STATUS (__stdcall *f_ftGetLibraryVersion)(LPDWORD lpdwDLLVersion);
typedef FT_STATUS (__stdcall *f_ftSetFlowControl)(FT_HANDLE ftHandle, USHORT usFlowControl, UCHAR uXon, UCHAR uXoff);

typedef FT_STATUS (__stdcall *f_ftGetDeviceInfoDetail)(DWORD dwIndex, LPDWORD lpdwFlags,
                                                       LPDWORD lpdwType,
                                                       LPDWORD lpdwID, LPDWORD lpdwLocId,
                                                       PCHAR pcSerialNumber, PCHAR pcDescription,
                                                       FT_HANDLE* ftHandle);


CFT2xxSerialPort::CFT2xxSerialPort(const std::string& name,
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
   m_connectStateEventId(shared::event::kNoEvent),
   m_connectRetryDelay(connectRetryDelay),
   m_flushAtConnect(flushAtConnect),
   m_isConnected(false),
   m_port(0)
{
   m_hGetProcIdDll = LoadLibraryA("ftd2xx.dll");

   if (!m_hGetProcIdDll)
   {
      const std::string message = "Could not load the dynamic library";
      YADOMS_LOG(error) << message;
      throw shared::exception::CException(message);
   }
   m_hEvent = CreateEventA(
      nullptr,
      false, // auto-reset event
      false, // non-signalled state
      "");

   if (m_hEvent == nullptr)
   {
      const std::string message = "Create event failed";
      YADOMS_LOG(error) << message;
      throw shared::exception::CException(message);
   }
}

void CFT2xxSerialPort::setPortNumber(int port)
{
   m_port = port;
}

std::vector<int> CFT2xxSerialPort::getPortComNumber()
{
   DWORD numDevs;
   const auto ftOpen = reinterpret_cast<f_ftopen>(GetProcAddress(m_hGetProcIdDll, "FT_Open"));
   const auto ftCreateDeviceInfoList = reinterpret_cast<f_ftCreateDeviceInfoList>(GetProcAddress(m_hGetProcIdDll, "FT_CreateDeviceInfoList"));
   const auto ftClose = reinterpret_cast<f_ftclose>(GetProcAddress(m_hGetProcIdDll, "FT_Close"));
   const auto ftGetComPortNumber = reinterpret_cast<f_ftGetComPortNumber>(GetProcAddress(m_hGetProcIdDll, "FT_GetComPortNumber"));

   YADOMS_LOG(information) << "Scan all FT2X ports ...";

   // create the device information list
   auto ftStatus = ftCreateDeviceInfoList(&numDevs);
   if (ftStatus == FT_OK)
   {
      YADOMS_LOG(information) << "Number of devices is " << numDevs;
   }
   else
   {
      YADOMS_LOG(error) << "FT_CreateDeviceInfoList failed";
   }

   for (unsigned char counter = 0; counter < numDevs; ++counter)
   {
      ftStatus = ftOpen(counter, &m_ftHandle);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to open the serial port with FTDI driver : ";
         YADOMS_LOG(error) << message << ftStatus;
         throw shared::exception::CException(message);
      }

      LONG lplComPortNumber = 0;
      ftStatus = ftGetComPortNumber(m_ftHandle, &lplComPortNumber);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to get the serial port number";
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }

      YADOMS_LOG(information) << "Port Com number : " << static_cast<int>(lplComPortNumber);
      m_serialPortComNumber.push_back(lplComPortNumber);

      if (m_ftHandle != nullptr)
      {
         ftClose(m_ftHandle);
         m_ftHandle = nullptr;
         m_isConnected = false;
      }
   }

   return m_serialPortComNumber;
}

CFT2xxSerialPort::~CFT2xxSerialPort()
{
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

   SetEvent(m_hEvent);
   m_asyncThread->join();
   m_asyncThread.reset();
   YADOMS_LOG(debug) << "CFT2xxSerialPort::stop()";
}

void CFT2xxSerialPort::receiverThread() const
{
   const auto ftGetStatus = reinterpret_cast<f_ftGetStatus>(GetProcAddress(m_hGetProcIdDll, "FT_GetStatus"));
   const auto ftGetModemStatus = reinterpret_cast<f_ftGetModemStatus>(GetProcAddress(m_hGetProcIdDll, "FT_GetModemStatus"));
   const auto ftRead = reinterpret_cast<f_ftRead>(GetProcAddress(m_hGetProcIdDll, "FT_Read"));

   YADOMS_LOG(debug) << "Create receiverThread";

   while (true)
   {
      WaitForSingleObject(m_hEvent, INFINITE);

      DWORD eventDWord;
      DWORD rxBytes;
      DWORD txBytes;
      DWORD status;
      DWORD bytesReceived;

      if (!m_isConnected) break;

      ftGetStatus(m_ftHandle, &rxBytes, &txBytes, &eventDWord);
      if (eventDWord & FT_EVENT_MODEM_STATUS)
      {
         // modem status event detected, so get current modem status
         ftGetModemStatus(m_ftHandle, &status);
         YADOMS_LOG(debug) << "Modem status event detected => exit";
         break;
      }
      if (rxBytes > 0)
      {
         char rxBuffer[2048] = { 0 };
         const auto ftStatus = ftRead(m_ftHandle, rxBuffer, rxBytes, &bytesReceived);

         // Read OK
         shared::communication::CByteBuffer buffer(bytesReceived);
         memcpy(buffer.begin(), rxBuffer, bytesReceived);

         if (ftStatus == FT_OK)
         {
            // FT_Read OK

            if (!!m_receiveBufferHandler && bytesReceived > 0)
               m_receiveBufferHandler->push(buffer);
         }
         else
         {
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
      const auto ftOpen = reinterpret_cast<f_ftopen>(GetProcAddress(m_hGetProcIdDll, "FT_Open"));
      const auto ftSetBaudRate = reinterpret_cast<f_ftsetbaudRate>(GetProcAddress(m_hGetProcIdDll, "FT_SetBaudRate"));
      const auto ftSetDataCharacteristics = reinterpret_cast<f_ftsetDataCharacteristics>(GetProcAddress(m_hGetProcIdDll, "FT_SetDataCharacteristics"));
      const auto ftCreateDeviceInfoList = reinterpret_cast<f_ftCreateDeviceInfoList>(GetProcAddress(m_hGetProcIdDll, "FT_CreateDeviceInfoList"));
      const auto ftGetDeviceInfoDetail = reinterpret_cast<f_ftGetDeviceInfoDetail>(GetProcAddress(m_hGetProcIdDll, "FT_GetDeviceInfoDetail"));

      DWORD flags;
      DWORD id;
      DWORD type;
      DWORD locId;

      DWORD numDevs;

      // create the device information list
      auto ftStatus = ftCreateDeviceInfoList(&numDevs);
      if (ftStatus == FT_OK)
      {
         YADOMS_LOG(information) << "Number of devices is " << numDevs;
      }
      else
      {
         YADOMS_LOG(error) << "FT_CreateDeviceInfoList failed";
      }

      if (numDevs > 0)
      {
         FT_HANDLE ftHandleTemp;
         char serialNumber[16];
         char description[64];

         // get information for device 0
         ftStatus = ftGetDeviceInfoDetail(m_port, &flags, &type, &id, &locId, serialNumber,
                                          description, &ftHandleTemp);
         if (ftStatus == FT_OK)
         {
            YADOMS_LOG(debug) << "Dev 0:";
            YADOMS_LOG(debug) << " Flags=" << flags;
            YADOMS_LOG(debug) << " Type=" << type;
            YADOMS_LOG(debug) << " ID=" << id;
            YADOMS_LOG(debug) << " LocId=" << locId;
            YADOMS_LOG(debug) << " SerialNumber=" << serialNumber;
            YADOMS_LOG(debug) << " Description=" << description;
            YADOMS_LOG(debug) << " ftHandle=" << ftHandleTemp;
         }
      }
      else
      {
         throw shared::exception::CException("No FTDI serial port detected");
      }

      ftStatus = ftOpen(m_port, &m_ftHandle);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to open the serial port with the FTDI driver";
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }

      DWORD dwDriverVersion;
      DWORD dwLibraryVersion;
      const auto ftGetDriverVersion = reinterpret_cast<f_ftGetDriverVersion>(GetProcAddress(m_hGetProcIdDll, "FT_GetDriverVersion"));
      const auto ftGetLibraryVersion = reinterpret_cast<f_ftGetLibraryVersion>(GetProcAddress(m_hGetProcIdDll, "FT_GetLibraryVersion"));
      const auto ftSetFlowControl = reinterpret_cast<f_ftSetFlowControl>(GetProcAddress(m_hGetProcIdDll, "FT_SetFlowControl"));


      ftStatus = ftGetDriverVersion(m_ftHandle, &dwDriverVersion);

      if (ftStatus != FT_OK)
      {
         std::string message = "Could not get the driver version";
         throw shared::exception::CException(message);
      }
      else
      {
         //convert int to hex string
         std::stringstream stream;
         stream << std::hex << ((dwDriverVersion & 0xFF0000) >> 16) << "."
            << std::hex << ((dwDriverVersion & 0x00FF00) >> 8) << "."
            << std::hex << (dwDriverVersion & 0x0000FF);
         auto hexNumber(stream.str());
         YADOMS_LOG(information) << "FTDI driver version : " << hexNumber;
      }

      ftGetLibraryVersion(&dwLibraryVersion);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Could not get the library version";
         throw shared::exception::CException(message);
      }
      //convert int to hex string
      std::stringstream stream;
      stream << std::hex << ((dwLibraryVersion & 0xFF0000) >> 16) << "."
         << std::hex << ((dwLibraryVersion & 0x00FF00) >> 8) << "."
         << std::hex << (dwLibraryVersion & 0x0000FF);
      auto hexNumber(stream.str());
      YADOMS_LOG(information) << "FTDI library version : " << hexNumber;

      ftStatus = ftSetBaudRate(m_ftHandle, m_baudrate.value());
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to set the baudrate";
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }

      UCHAR stopBits;
      UCHAR parity;

      if (m_stop_bits.value() == boost::asio::serial_port_base::stop_bits::one)
         stopBits = FT_STOP_BITS_1;
      else if (m_stop_bits.value() == boost::asio::serial_port_base::stop_bits::two)
         stopBits = FT_STOP_BITS_2;
      else
         stopBits = FT_STOP_BITS_1;

      if (m_parity.value() == boost::asio::serial_port_base::parity::even)
         parity = FT_PARITY_EVEN;
      else if (m_parity.value() == boost::asio::serial_port_base::parity::odd)
         parity = FT_PARITY_ODD;
      else if (m_parity.value() == boost::asio::serial_port_base::parity::none)
         parity = FT_PARITY_NONE;
      else
         parity = FT_PARITY_NONE;

      ftStatus = ftSetDataCharacteristics(m_ftHandle, static_cast<UCHAR>(m_characterSize.value()), stopBits, parity);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to set Data format";
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }

      USHORT flowControl;

      if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::hardware)
         flowControl = FT_FLOW_XON_XOFF;
      else if (m_flowControl.value() == boost::asio::serial_port_base::flow_control::none)
         flowControl = FT_FLOW_NONE;
      else
         flowControl = FT_FLOW_NONE;

      ftStatus = ftSetFlowControl(m_ftHandle, flowControl, 0x00, 0x00);
      if (ftStatus != FT_OK)
      {
         const std::string message = "Fail to set Flow Control";
         YADOMS_LOG(error) << message;
         throw shared::exception::CException(message);
      }

      m_isConnected = true;
   }
   catch (boost::system::system_error& e)
   {
      notifyEventHandler("asyncPort.serial.failToOpen", {{"port", m_name}});
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
      const auto ftClose = reinterpret_cast<f_ftclose>(GetProcAddress(m_hGetProcIdDll, "FT_Close"));

      if (m_ftHandle != nullptr)
      {
         m_isConnected = false;
         ftClose(m_ftHandle);
         m_ftHandle = nullptr;
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

void CFT2xxSerialPort::subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler,
                                                    int forId)
{
   m_connectStateEventHandler = &forEventHandler;
   m_connectStateEventId = forId;
}

void CFT2xxSerialPort::setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler> handler)
{
   m_receiveBufferHandler = handler;
}

void CFT2xxSerialPort::flush()
{
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
         SetEvent(m_hEvent);
         m_asyncThread->join();
         m_asyncThread.reset();
         notifyEventHandler("asyncPort.serial.failToOpen", {{"port", m_name}});
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

void CFT2xxSerialPort::startRead() const
{
   const auto ftSetEventNotification = reinterpret_cast<f_ftsetEventNotification>(GetProcAddress(m_hGetProcIdDll, "FT_SetEventNotification"));
   const DWORD eventMask = FT_EVENT_RXCHAR | FT_EVENT_MODEM_STATUS;

   const auto ftStatus = ftSetEventNotification(m_ftHandle, eventMask, m_hEvent);

   if (ftStatus != FT_OK)
   {
      const std::string message = "Fail to set event notification";
      YADOMS_LOG(error) << message;
      throw shared::exception::CException(message);
   }
}

void CFT2xxSerialPort::activateGPIO(const int GPIONumber) const
{
   unsigned char ucMask;

   const auto ftSetBitMode = reinterpret_cast<f_ftsetBitMode>(GetProcAddress(m_hGetProcIdDll, "FT_SetBitMode"));
   const auto ftGetBitMode = reinterpret_cast<f_ftgetBitMode>(GetProcAddress(m_hGetProcIdDll, "FT_GetBitMode"));

   switch (GPIONumber)
   {
   case 1:
      ucMask = MaskPort1;
      break;
   case 2:
      ucMask = MaskPort2;
      break;
   default:
      ucMask = MaskNoPort;
   }

   auto ucMode = EnableBitBang;

   auto ftStatus = ftSetBitMode(m_ftHandle, ucMask, ucMode);

   if (ftStatus != FT_OK)
      YADOMS_LOG(error) << "Failed to set bit mode for port: " << GPIONumber;
   else
      YADOMS_LOG(debug) << "Set bit mode for port: " << GPIONumber;

   ftStatus = ftGetBitMode(m_ftHandle, &ucMode);
   if (ftStatus != FT_OK)
   {
      YADOMS_LOG(error) << "Failed to get bit mode";
   }
}

void CFT2xxSerialPort::desactivateGPIO() const
{
   // No GPIO activate => No access
   activateGPIO(0);
}

void CFT2xxSerialPort::send(const shared::communication::CByteBuffer& buffer)
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
      const auto param = boost::make_shared<shared::communication::CAsyncPortConnectionNotification>();
      m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
   }
}

void CFT2xxSerialPort::notifyEventHandler(const std::string& i18nErrorMessage) const
{
   if (m_connectStateEventHandler)
   {
      const auto param = boost::make_shared<shared::communication::CAsyncPortConnectionNotification>(i18nErrorMessage);
      m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
   }
}

void CFT2xxSerialPort::notifyEventHandler(const std::string& i18nErrorMessage,
                                          const std::map<std::string, std::string>& i18nMessageParameters) const
{
   if (m_connectStateEventHandler)
   {
      const auto param = boost::make_shared<shared::communication::CAsyncPortConnectionNotification>(
         i18nErrorMessage, i18nMessageParameters);
      m_connectStateEventHandler->postEvent(m_connectStateEventId, param);
   }
}

void CFT2xxSerialPort::setBaudRate(
   const boost::asio::serial_port_base::baud_rate& baudrate = boost::asio::serial_port_base::baud_rate(9600))
{
   const auto ftSetBaudRate = reinterpret_cast<f_ftsetbaudRate>(GetProcAddress(m_hGetProcIdDll, "FT_SetBaudRate"));
   m_baudrate = baudrate;

   YADOMS_LOG(information) << m_baudrate.value();

   const auto ftStatus = ftSetBaudRate(m_ftHandle, m_baudrate.value());
   if (ftStatus != FT_OK)
   {
      const std::string message = "Fail to set the baudrate";
      YADOMS_LOG(error) << message;
      throw shared::exception::CException(message);
   }
}
