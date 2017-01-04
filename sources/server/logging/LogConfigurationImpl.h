#pragma once
#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>


namespace logging
{
   class CLogConfigurationImpl
   {
   public:
      CLogConfigurationImpl();
      virtual ~CLogConfigurationImpl();
      void configure(const std::string & logLevel);
   private:
      Poco::AutoPtr<Poco::ConsoleChannel> m_consoleChannel;
      Poco::AutoPtr<Poco::FileChannel> m_fileChannel;
      Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingConsoleChannel;
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
      Poco::AutoPtr<Poco::SplitterChannel> m_splitterChannel;
   };
} // namespace logging