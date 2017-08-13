#pragma once

#include <string.h>
#include <vector>

namespace testCommon
{
   enum
   {
      kSTX = 0x02,
      kETX = 0x03,

      kStartMessage = 0x0a,
      kHorizontalTab = 0x09,
      kEndMessage = 0x0d
   };

   class serialTeleInfoMessage
   {
   public:

      static std::string toMessage(const std::string& content);
      static std::vector<unsigned char> normalizeFrame(const std::string& content);
   };
} // namespace testCommon