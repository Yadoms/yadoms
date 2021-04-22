#pragma once

namespace hardware
{
   namespace serial
   {
      class CSerialPortsLister
      {
      public:
         CSerialPortsLister() = default;
         virtual ~CSerialPortsLister() = default;

          //--------------------------------------------------------------
          /// \brief			List machine serial ports (all)
          /// \return       The serial ports names (keys are real name, values are common name or comment ("used by..."))
          //--------------------------------------------------------------
          typedef std::map<std::string, std::string> SerialPortsMap;
          static boost::shared_ptr<SerialPortsMap> listSerialPorts();
      };
   } // namespace serial
} // namespace hardware