#pragma once
#include "Poco/Channel.h"
#include "Poco/Mutex.h"


namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      ///\brief A dual-console log channel, to output to std::cout and std::cerr depending on log level
      //--------------------------------------------------------------
      class CoutCerrConsoleChannel: public Poco::Channel
      {
      public:
         CoutCerrConsoleChannel();

         void log(const Poco::Message& msg) override;

      protected:
         virtual ~CoutCerrConsoleChannel();

      private:
         static Poco::FastMutex _mutex;
      };
   }
} // namespace shared::process