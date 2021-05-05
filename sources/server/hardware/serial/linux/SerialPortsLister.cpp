#include "stdafx.h"
#include <shared/Log.h>
#include "SerialPortsLister.h"
#include <termios.h>

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSerialPorts()
      {
         auto physicalSerialPorts = listPhysicalSerialPorts();
         auto symbolicLinksToSerialPorts = listSymbolicLinksToSerialPorts();

         auto serialPorts(boost::make_shared<SerialPortsMap>());
         serialPorts.insert(physicalSerialPorts.begin(), physicalSerialPorts.end());
         serialPorts.insert(symbolicLinksToSerialPorts.begin(), symbolicLinksToSerialPorts.end());

         return serialPorts;
      }

      boost::shared_ptr<SerialPortsMap> listPhysicalSerialPorts()
      {
         boost::filesystem::path ttyDir("/sys/class/tty");

         auto serialPorts(boost::make_shared<SerialPortsMap>());

         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
               {
                  std::string friendlyName((*dirIter).path().leaf().string());            // friendlyName is something like "tty0"
                  std::string portName((boost::format("/dev/%1%") % friendlyName).str()); // portName is "/dev/tty0"
                  (*serialPorts)[portName]=friendlyName;
               }
            }
         }

         return serialPorts;
      }

      boost::shared_ptr<SerialPortsMap> listSymbolicLinksToSerialPorts()
      {
         boost::filesystem::path ttyDir("/dev");

         auto serialPorts(boost::make_shared<SerialPortsMap>());

         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               if (boost::filesystem::is_symlink(*dirIter))
               {
                  std::string friendlyName(dirIter->leaf().string()); // friendlyName comes from udev rules (ex : "ttyUSB_EnOcean")
                  std::string portName(dirIter->string());            // portName is "/dev/ttyUSB_EnOcean"
                  (*serialPorts)[portName]=friendlyName;
               }
            }
         }

         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
