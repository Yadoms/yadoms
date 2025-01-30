#include "stdafx.h"
#include <shared/Log.h>
#include "SerialPortsLister.h"

namespace hardware
{
   namespace serial
   {
      boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSerialPorts()
      {
         YADOMS_LOG(debug) << "List system serial ports...";

         auto physicalSerialPorts = listPhysicalSerialPorts();
         auto symbolicLinksToSerialPorts = listSymbolicLinksToSerialPorts(physicalSerialPorts);

         auto serialPorts = boost::make_shared<SerialPortsMap>();
         serialPorts->insert(physicalSerialPorts->begin(), physicalSerialPorts->end());
         serialPorts->insert(symbolicLinksToSerialPorts->begin(), symbolicLinksToSerialPorts->end());

         YADOMS_LOG(information) << "Serial ports found :";
         for (const auto& sp : *serialPorts)
            YADOMS_LOG(information) << "  - " << sp.first << ", " << sp.second;

         return serialPorts;
      }

      boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listPhysicalSerialPorts()
      {
         boost::filesystem::path ttyDir("/sys/class/tty");

         auto serialPorts = boost::make_shared<SerialPortsMap>();

         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               if (boost::filesystem::is_directory(*dirIter) && boost::filesystem::exists(*dirIter / "device"))
               {
                  std::string friendlyName((*dirIter).path().filename().string());            // friendlyName is something like "tty0"
                  std::string portName((boost::format("/dev/%1%") % friendlyName).str()); // portName is "/dev/tty0"
                  (*serialPorts)[portName]=friendlyName;
               }
            }
         }

         return serialPorts;
      }

      boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> CSerialPortsLister::listSymbolicLinksToSerialPorts(boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> physicalPorts)
      {
         boost::filesystem::path ttyDir("/dev");

         auto serialPorts = boost::make_shared<SerialPortsMap>();
         
         if (boost::filesystem::exists(ttyDir) && boost::filesystem::is_directory(ttyDir))
         {
            boost::filesystem::directory_iterator endIter;
            for (boost::filesystem::directory_iterator dirIter(ttyDir) ; dirIter != endIter ; ++dirIter)
            {
               if (boost::filesystem::is_symlink(*dirIter) && linkTargetIsPhysicalPort(*dirIter, physicalPorts))
               {
                  std::string friendlyName(dirIter->path().filename().string()); // friendlyName comes from udev rules (ex : "ttyUSB_EnOcean")
                  std::string portName(dirIter->path().string());            // portName is "/dev/ttyUSB_EnOcean"
                  (*serialPorts)[portName]=friendlyName;
               }
            }
         }
         
         return serialPorts;
      }

      bool CSerialPortsLister::linkTargetIsPhysicalPort(const boost::filesystem::path& link,
                                                        boost::shared_ptr<const CSerialPortsLister::SerialPortsMap> physicalPorts)
      {
         const auto linkTarget = read_symlink(link);
         return std::find_if(physicalPorts->begin(),
                             physicalPorts->end(),
                             [&linkTarget](const auto& pp) {
                                return boost::filesystem::path(pp.first).filename() == linkTarget.filename();
                             }) != physicalPorts->end();
      }

   } // namespace serial
} // namespace hardware
