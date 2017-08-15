#include "stdafx.h"
#include "serialTeleInfoMessage.h"

namespace testCommon
{
   std::string serialTeleInfoMessage::toMessage(const std::string& content)
   {
      std::string message;
      message.push_back(kStartMessage);
      for (const auto car : content)
         message.push_back(car);
      message.push_back(kEndMessage);

      // replace internal characters of the message if any
      boost::replace_all(message, "<ht>", std::string(1, kHorizontalTab));

      return message;
   }

   std::vector<unsigned char> serialTeleInfoMessage::normalizeFrame(const std::string& content)
   {
      auto outString = content;
      boost::replace_all(outString, "<etx>", std::string(1, kETX));
      boost::replace_all(outString, "<stx>", std::string(1, kSTX));
      boost::replace_all(outString, "<lf>", std::string(1, kStartMessage));
      boost::replace_all(outString, "<cr>", std::string(1, kEndMessage));
      boost::replace_all(outString, "<ht>", std::string(1, kHorizontalTab));

      std::vector<unsigned char> outVector;
      for (const auto car : outString)
         outVector.push_back(static_cast<unsigned char>(car));

      return outVector;
   }
} // namespace testCommon
