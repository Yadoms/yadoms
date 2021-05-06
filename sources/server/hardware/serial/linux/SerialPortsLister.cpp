#include "stdafx.h"
#include <shared/Log.h>
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSerialPorts()
      {
         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts()...";
         
         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts(), call listPhysicalSerialPorts...";
         auto physicalSerialPorts = listPhysicalSerialPorts();
         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts(), call listSymbolicLinksToSerialPorts...";
         auto symbolicLinksToSerialPorts = listSymbolicLinksToSerialPorts();

         auto serialPorts(boost::make_shared<SerialPortsMap>());
         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts(), insert physicalSerialPorts...";
         serialPorts->insert(physicalSerialPorts->begin(), physicalSerialPorts->end());
         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts(), insert symbolicLinksToSerialPorts...";
         serialPorts->insert(symbolicLinksToSerialPorts->begin(), symbolicLinksToSerialPorts->end());

         YADOMS_LOG(debug) << "CSerialPortsLister::listSerialPorts() => found " << serialPorts.size() << " ports";
         return serialPorts;
      }

      boost::shared_ptr<CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listPhysicalSerialPorts()
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

      boost::shared_ptr<CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSymbolicLinksToSerialPorts()
      {
         YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts()...";

         boost::filesystem::path ttyDir("/dev");

         auto serialPorts(boost::make_shared<SerialPortsMap>());
         
         YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts(), if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir)) ...";
         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts(), for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter) ..";
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts(), if (boost::filesystem::is_symlink(*dirIter)) ...";
               if (boost::filesystem::is_symlink(*dirIter))
               {
                  YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts(), std::string friendlyName(dirIter->path().leaf().string()); ...";
                  std::string friendlyName(dirIter->path().leaf().string()); // friendlyName comes from udev rules (ex : "ttyUSB_EnOcean")
                  YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts(), std::string portName(dirIter->path().string()); ...";
                  std::string portName(dirIter->path().string());            // portName is "/dev/ttyUSB_EnOcean"
                  YADOMS_LOG(debug) << "Found SL serial port " << friendlyName << ", " << portName;
                  (*serialPorts)[portName]=friendlyName;
               }
            }
         }
         
         YADOMS_LOG(debug) << "CSerialPortsLister::listSymbolicLinksToSerialPorts() => found " << serialPorts.size() << " ports";
         return serialPorts;
      }
   } // namespace serial
} // namespace hardware
