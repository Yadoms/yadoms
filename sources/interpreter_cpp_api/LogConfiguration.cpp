#include "stdafx.h"
#include "LogConfiguration.h"
#include <Poco/Logger.h>


namespace interpreter_cpp_api
{
   CLogConfiguration::CLogConfiguration()
      : m_consolePatternFormatter(new Poco::PatternFormatter),
        m_consoleChannel(new shared::process::CCoutCerrConsoleLogChannel),
        m_patternFormatter(new Poco::PatternFormatter),
        m_fileChannel(new Poco::FileChannel()),
        m_splitterChannel(new Poco::SplitterChannel)
   {
   }

   CLogConfiguration::~CLogConfiguration()
   {
   }

   void CLogConfiguration::configure(const std::string& logLevel,
                                     boost::filesystem::path& logfilepath)
   {
      m_consolePatternFormatter->setProperty("pattern", "[%p]%T : %t");
      m_formattingConsoleChannel.assign(new Poco::FormattingChannel(m_consolePatternFormatter,
                                                                    m_consoleChannel));

      m_patternFormatter->setProperty("pattern", "%H:%M:%S : %T : [%p] : %t");
      m_patternFormatter->setProperty("times", "local"); //use local datetime

      if (!boost::filesystem::exists(logfilepath.parent_path().string()))
         if (!boost::filesystem::create_directories(logfilepath.parent_path().string()))
            throw std::runtime_error((boost::format("Cannot create directory %1%") % logfilepath.parent_path()).str());

      m_fileChannel->setProperty("path", logfilepath.string());
      m_fileChannel->setProperty("rotation", "daily");
      m_fileChannel->setProperty("archive", "timestamp");
      m_fileChannel->setProperty("compress", "true");
      m_fileChannel->setProperty("purgeCount", "7");
      m_formattingFileChannel.assign(new Poco::FormattingChannel(m_patternFormatter,
                                                                 m_fileChannel));

      m_splitterChannel->addChannel(m_formattingConsoleChannel);
      m_splitterChannel->addChannel(m_formattingFileChannel);

      //configre any already created loggers
      std::vector<std::string> loggerNames;
      Poco::Logger::names(loggerNames);
      for (const auto& loggerName : loggerNames)
      {
         Poco::Logger::get(loggerName).setChannel(m_splitterChannel);
         Poco::Logger::get(loggerName).setLevel(logLevel);
      }
      loggerNames.clear();

      //configure root logger
      Poco::Logger::root().setChannel(m_splitterChannel);
      Poco::Logger::root().setLevel(logLevel);
   }
} // namespace interpreter_cpp_api
