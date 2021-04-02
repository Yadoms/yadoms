#pragma once

namespace hardware
{
   namespace serial
   {
      class IManuallyDefinedPortLister
      {
      public:
         virtual ~IManuallyDefinedPortLister() = default;

         //--------------------------------------------------------------
         /// \brief			List all manually define serial ports
         /// \return       The serial ports names
         //--------------------------------------------------------------
         virtual std::vector<std::string>> listSerialPorts() = 0;
      };
   } // namespace serial
} // namespace hardware
