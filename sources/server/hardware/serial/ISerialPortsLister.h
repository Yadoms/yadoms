#pragma once

namespace hardware
{
   namespace serial
   {
      class ISerialPortsLister
      {
      public:
         virtual ~ISerialPortsLister() = default;

         typedef std::map<std::string, std::string> SerialPortsMap;

         //--------------------------------------------------------------
         /// \brief			List machine serial ports (all)
         /// \return       The serial ports names (keys are real name, values are common name)
         //--------------------------------------------------------------
         virtual boost::shared_ptr<const SerialPortsMap> listSerialPorts() = 0;
      };
   } // namespace serial
} // namespace hardware
