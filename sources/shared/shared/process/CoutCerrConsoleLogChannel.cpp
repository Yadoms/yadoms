#include "stdafx.h"
#include "CoutCerrConsoleLogChannel.h"
#include <Poco/Message.h>


namespace shared
{
   namespace process
   {
      Poco::FastMutex CCoutCerrConsoleLogChannel::_mutex;


      CCoutCerrConsoleLogChannel::CCoutCerrConsoleLogChannel()
      {
      }


      CCoutCerrConsoleLogChannel::~CCoutCerrConsoleLogChannel()
      {
      }


      void CCoutCerrConsoleLogChannel::log(const Poco::Message& msg)
      {
         Poco::FastMutex::ScopedLock lock(_mutex);

         if (msg.getPriority() <= Poco::Message::PRIO_ERROR)
            std::cerr << msg.getText() << std::endl;
         else
            std::cout << msg.getText() << std::endl;
      }
   }
} // namespace shared::process