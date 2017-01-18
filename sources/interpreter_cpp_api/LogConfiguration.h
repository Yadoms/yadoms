#pragma once
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include "shared/process/CoutCerrConsoleLogChannel.h"
#include <Poco/FileChannel.h>
#include <Poco/SplitterChannel.h>

namespace interpreter_cpp_api
{
   //--------------------------------------------------------------
   /// \brief The specific log configuration for interpreters
   //--------------------------------------------------------------
   class CLogConfiguration
   {
   public:
      CLogConfiguration();
      virtual ~CLogConfiguration();

      void configure(const std::string& logLevel,
                     boost::filesystem::path& logfilepath);
   private:
      Poco::AutoPtr<Poco::PatternFormatter> m_consolePatternFormatter;
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingConsoleChannel;
      Poco::AutoPtr<shared::process::CCoutCerrConsoleLogChannel> m_consoleChannel;

      Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
      Poco::AutoPtr<Poco::FileChannel> m_fileChannel;

      Poco::AutoPtr<Poco::SplitterChannel> m_splitterChannel;
   };
} // namespace interpreter_cpp_api


