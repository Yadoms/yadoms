#include "StandardConsoleChannel.h"
#include "Poco/Message.h"

Poco::FastMutex StandardConsoleChannel::_mutex;


StandardConsoleChannel::StandardConsoleChannel()
{
}


StandardConsoleChannel::~StandardConsoleChannel()
{
}


void StandardConsoleChannel::log(const Poco::Message& msg)
{
   Poco::FastMutex::ScopedLock lock(_mutex);

   if (msg.getPriority() <= Poco::Message::PRIO_ERROR)
      std::cerr << msg.getText() << std::endl;
   else
      std::cout << msg.getText() << std::endl;
}
