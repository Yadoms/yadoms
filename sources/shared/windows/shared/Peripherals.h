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
      static const boost::shared_ptr<SerialPortsMap> getSerialPorts();

   private:
      //--------------------------------------------------------------
      /// \brief			List used serial ports
      /// \return       The used serial ports names, Windows-standard named ("COM1", "COM3"...)
      //--------------------------------------------------------------
      static const boost::shared_ptr<std::set<std::string> > getUsedSerialPorts();
   };

} // namespace shared