#pragma once

namespace message
{
   std::string deviceIdToString(unsigned int deviceId)
   {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(8) << std::uppercase << std::hex << deviceId;
      return ss.str();
   }
} // namespace message
