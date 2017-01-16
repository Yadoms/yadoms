#pragma once
#include "Poco/Channel.h"
#include "Poco/Mutex.h"

//--------------------------------------------------------------
///\brief A standard console log channel, to output to std::cout and std::cerr
//--------------------------------------------------------------
class StandardConsoleChannel: public Poco::Channel//TODO mettre dans shared
{
public:
   StandardConsoleChannel();

   void log(const Poco::Message& msg) override;

protected:
   virtual ~StandardConsoleChannel();

private:
   static Poco::FastMutex _mutex;
};
