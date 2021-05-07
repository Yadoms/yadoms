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
         static boost::shared_ptr<const SerialPortsMap> listSerialPorts();

      private:
         static boost::shared_ptr<const SerialPortsMap> listPhysicalSerialPorts();
         static boost::shared_ptr<const SerialPortsMap> listSymbolicLinksToSerialPorts(boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> physicalPorts);
         static bool linkTargetIsPhysicalPort(const boost::filesystem::path& linkTarget,
                                              boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> physicalPorts);
      };
   } // namespace serial
} // namespace hardware