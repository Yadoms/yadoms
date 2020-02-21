#include "stdafx.h"
#include "LsusbDevicesLister.h"
#include "LsusbDevice.h"
#include "LsusbCall.h"
#include <shared/Log.h>
#include <regex>

namespace hardware
{
namespace usb
{
std::vector<boost::shared_ptr<IDevice>> CLsusbDevicesLister::listUsbDevices()
{
   try
   {
      CLsusbCall libusbCall;
      const auto lines = libusbCall.execute();

      std::vector<boost::shared_ptr<IDevice>> devicesList;
      for (const auto &line : lines)
      {
         try
         {
            std::smatch matches;
            if (!std::regex_search(line,
                                   matches,
                                   std::regex(std::string("^Bus.*: ID ([[:xdigit:]]{4}):([[:xdigit:]]{4}) (.*)$"))) ||
                matches.size() != 4)
               continue;

            const auto vid = std::stoi(std::string(matches[1].first, matches[1].second), nullptr, 16);
            const auto pid = std::stoi(std::string(matches[2].first, matches[2].second), nullptr, 16);
            const auto nameWithSerial = std::string(matches[3]);
            
            if (!std::regex_search(nameWithSerial,
                                   matches,
                                   std::regex(std::string("^(.*) Serial: (.*)$"))) ||
                matches.size() != 3)
               continue;

            const auto name = matches.size() != 3 ? nameWithSerial : matches[1];
            const auto serial = matches.size() != 3 ? std::string() : matches[2];

            devicesList.emplace_back(boost::make_shared<CLsusbDevice>(vid,
                                                                      pid,
                                                                      name,
                                                                      serial));
         }
         catch (const std::exception &e)
         {
            YADOMS_LOG(warning) << "Unable to access device " << e.what();
         }
      }
      return devicesList;
   }
   catch (const std::exception &e)
   {
      YADOMS_LOG(warning) << "Unable to list USB devices";
      return std::vector<boost::shared_ptr<IDevice>>();
   }
}
} // namespace usb
} // namespace hardware
