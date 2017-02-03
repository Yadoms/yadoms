//
// Peripherals.h
//
// Platforms peripherals management (serial ports...)
//
#pragma once

namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms peripherals
   //--------------------------------------------------------------
   class CPeripherals
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Constructor
      //--------------------------------------------------------------
      CPeripherals();

      //--------------------------------------------------------------
      /// \brief			Destructor
      //--------------------------------------------------------------
      virtual ~CPeripherals();

      //--------------------------------------------------------------
      /// \brief			List machine serial ports (all)
      /// \return       The serial ports names (keys are real name, values are common name or comment ("used by..."))
      //--------------------------------------------------------------
      typedef std::map<std::string, std::string> SerialPortsMap;
      static boost::shared_ptr<SerialPortsMap> getSerialPorts();

      //--------------------------------------------------------------
      /// \brief			Flush serial port buffers (input & output)
      /// \param[in]    port The port to flush
      /// \return       error code
      //--------------------------------------------------------------
      static boost::system::error_code flushSerialPort(boost::asio::serial_port& port);
   };

} // namespace shared