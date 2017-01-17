#include "stdafx.h"
#include "CoutCerrConsoleChannel.h"
#include "Poco/Message.h"


namespace shared
{
   namespace process
   {
      Poco::FastMutex CoutCerrConsoleChannel::_mutex;


      CoutCerrConsoleChannel::CoutCerrConsoleChannel()
      {
      }


      CoutCerrConsoleChannel::~CoutCerrConsoleChannel()
      {
      }


      void CoutCerrConsoleChannel::log(const Poco::Message& msg)
      {
         Poco::FastMutex::ScopedLock lock(_mutex);

         if (msg.getPriority() <= Poco::Message::PRIO_ERROR)
            std::cerr << msg.getText() << std::endl;
         else
            std::cout << msg.getText() << std::endl;
      }
   }
} // namespace shared::process