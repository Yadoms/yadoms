#pragma once
#include <Poco/Channel.h>
#include <Poco/Mutex.h>


namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      ///\brief A dual-console log channel, to output to std::cout and std::cerr depending on log level
      //--------------------------------------------------------------
      class CCoutCerrConsoleLogChannel: public Poco::Channel
      {
      public:
         CCoutCerrConsoleLogChannel();

         void log(const Poco::Message& msg) override;

      protected:
         virtual ~CCoutCerrConsoleLogChannel();

      private:
         static Poco::FastMutex _mutex;
      };
   }
} // namespace shared::process