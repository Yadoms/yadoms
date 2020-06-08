#include "stdafx.h"
#include "SystemProfilerDevicesLister.h"
#include "SystemProfilerDevice.h"
#include "SystemProfilerCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
std::vector<boost::shared_ptr<IDevice>> CSystemProfilerDevicesLister::listUsbDevices()
{
   try
   {
      std::vector<std::string> args;
      args.push_back("SPUSBDataType");
 
      CSystemProfilerCall SystemProfilerCall(args);
      const auto lines = SystemProfilerCall.execute(true);

      std::vector<boost::shared_ptr<IDevice>> devicesList;
      int vid = 0;
      int pid = 0;
      std::string name;
      std::string serialNumber;
      for (const auto &line : lines)
      {
         try
         {
            std::smatch matches;
            if (!std::regex_search(line,
                                   matches,
                                   std::regex(std::string("Product ID: (0x?[[:xdigit:]]{4})|Vendor ID: (0x?[[:xdigit:]]{4}) \\s*\\(([^)]*)|Serial Number: ([a-zA-Z0-9]+)"))))
               continue;
             
            if(matches[1].matched)
                pid = std::stoi(std::string(matches[1].first, matches[1].second), nullptr, 16);
            if(matches[2].matched)
                vid = std::stoi(std::string(matches[2].first, matches[2].second), nullptr, 16);
            if(matches[3].matched)
                name = matches[3];
            if(matches[4].matched)
                serialNumber = matches[4];
             if(pid != 0  && vid != 0 && !name.empty() && !serialNumber.empty())
            {
                devicesList.emplace_back(boost::make_shared<CSystemProfilerDevice>(vid,
                                                                          pid,
                                                                          name,
                                                                          serialNumber));
                pid = 0;
                vid = 0;;
                name.clear();
                serialNumber.clear();
            }

         }
         catch (const std::exception &e)
         {
            YADOMS_LOG(warning) << "Unable to access device, " << e.what();
         }
      }
      return devicesList;
   }
   catch (const std::exception &e)
   {
      YADOMS_LOG(warning) << "Unable to list USB devices, " << e.what();
      return std::vector<boost::shared_ptr<IDevice>>();
   }
}
} // namespace usb
} // namespace hardware
