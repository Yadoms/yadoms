#include "stdafx.h"
#include "AsciiBufferLogger.h"
#include <shared/Log.h>

namespace shared
{
   namespace communication
   {
      CAsciiBufferLogger::CAsciiBufferLogger()
      {
      }

      CAsciiBufferLogger::~CAsciiBufferLogger()
      {
      }

      void CAsciiBufferLogger::logReceived(const CByteBuffer& data)
      {
         YADOMS_LOG(information) << "Yadoms <<< " << msgToString(data) << std::endl;
      }

      void CAsciiBufferLogger::logSent(const CByteBuffer& data)
      {
         YADOMS_LOG(information) << "Yadoms >>> " << msgToString(data) << std::endl;
      }

      std::string CAsciiBufferLogger::msgToString(const CByteBuffer& data)
      {
         std::ostringstream ss;
         for (size_t idx = 0; idx < data.size(); ++ idx)
         {
            char c = data[idx];
            switch (c)
            {
            case 0x00: ss << "<nul>";
               break;
            case 0x01: ss << "<soh>";
               break;
            case 0x02: ss << "<stx>";
               break;
            case 0x03: ss << "<etx>";
               break;
            case 0x04: ss << "<eot>";
               break;
            case 0x05: ss << "<enq>";
               break;
            case 0x06: ss << "<ack>";
               break;
            case 0x07: ss << "<bel>";
               break;
            case 0x08: ss << "<bs>";
               break;
            case 0x09: ss << "<tab>";
               break;
            case 0x0A: ss << "<lf>";
               break;
            case 0x0B: ss << "<vt>";
               break;
            case 0x0C: ss << "<ff>";
               break;
            case 0x0D: ss << "<cr>";
               break;
            case 0x0E: ss << "<so>";
               break;
            case 0x0F: ss << "<si>";
               break;
            case 0x10: ss << "<dle>";
               break;
            case 0x11: ss << "<dc1>";
               break;
            case 0x12: ss << "<dc2>";
               break;
            case 0x13: ss << "<dc3>";
               break;
            case 0x14: ss << "<dc4>";
               break;
            case 0x15: ss << "<nak>";
               break;
            case 0x16: ss << "<syn>";
               break;
            case 0x17: ss << "<etb>";
               break;
            case 0x18: ss << "<can>";
               break;
            case 0x19: ss << "<em>";
               break;
            case 0x1A: ss << "<sub>";
               break;
            case 0x1B: ss << "<esc>";
               break;
            case 0x1C: ss << "<fs>";
               break;
            case 0x1D: ss << "<gs>";
               break;
            case 0x1E: ss << "<rs>";
               break;
            case 0x1F: ss << "<us>";
               break;
            case 0x7F: ss << "<del>";
               break;
            default: ss << c;
               break;
            }
         }

         return ss.str();
      }
   }
} // namespace shared::communication


