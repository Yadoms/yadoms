#include "stdafx.h"
#include "InterpreterLogConfiguration.h"
#include <Poco/Logger.h>
#include <shared/exception/Exception.hpp>

namespace interpreter_cpp_api
{
   CInterpreterLogConfiguration::CInterpreterLogConfiguration()
      : m_consoleChannel(new Poco::ConsoleChannel),
        m_fileChannel(new Poco::FileChannel()),
        m_patternFormatter(new Poco::PatternFormatter),
        m_splitterChannel(new Poco::SplitterChannel)
   {
   }

   CInterpreterLogConfiguration::~CInterpreterLogConfiguration()
   {
   }

   void CInterpreterLogConfiguration::configure(const std::string& logLevel,
                                                boost::filesystem::path& logfilepath)
   {
      //make pattern
      m_patternFormatter->setProperty("pattern", "%H:%M:%S : %T : [%p] : %t");
      m_patternFormatter->setProperty("times", "local"); //use local datetime

      m_formattingConsoleChannel.assign(new Poco::FormattingChannel(m_patternFormatter, m_consoleChannel));

      if (!boost::filesystem::exists(logfilepath.string()))
         if (!boost::filesystem::create_directories(logfilepath.parent_path().string()))
            throw shared::exception::CException("Cannot create directory " + logfilepath.parent_path().string());

      m_fileChannel->setProperty("path", logfilepath.string());
      m_fileChannel->setProperty("rotation", "daily");
      m_fileChannel->setProperty("archive", "timestamp");
      m_fileChannel->setProperty("compress", "true");
      m_fileChannel->setProperty("purgeCount", "7");
      m_formattingFileChannel.assign(new Poco::FormattingChannel(m_patternFormatter, m_fileChannel));

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


