#include "stdafx.h"
#include "YadomsSubModuleLogConfiguration.h"
#include <Poco/Logger.h>

namespace shared
{
   namespace process
   {
      CYadomsSubModuleLogConfiguration::CYadomsSubModuleLogConfiguration()
         : m_patternFormatter(new Poco::PatternFormatter),
           m_fileChannel(new Poco::FileChannel())
      {
      }

      CYadomsSubModuleLogConfiguration::~CYadomsSubModuleLogConfiguration()
      {
      }

      void CYadomsSubModuleLogConfiguration::configure(const std::string& logLevel,
                                                       boost::filesystem::path& logfilepath)
      {
         m_patternFormatter->setProperty("pattern", "%H:%M:%S : %T : [%p] : %t");
         m_patternFormatter->setProperty("times", "local"); //use local datetime

         if (!boost::filesystem::exists(logfilepath.parent_path().string()))
            if (!boost::filesystem::create_directories(logfilepath.parent_path().string()))
               throw std::runtime_error((boost::format("Cannot create directory %1%") % logfilepath.parent_path()).str());

         m_fileChannel->setProperty("times", "local"); //use local datetime for rotation strategy
         m_fileChannel->setProperty("path", logfilepath.string());
         m_fileChannel->setProperty("rotation", "daily");
         m_fileChannel->setProperty("archive", "timestamp");
         m_fileChannel->setProperty("compress", "true");
         m_fileChannel->setProperty("purgeCount", "7");
         m_formattingFileChannel.assign(new Poco::FormattingChannel(m_patternFormatter,
                                                                    m_fileChannel));

         //configure any already created loggers
         std::vector<std::string> loggerNames;
         Poco::Logger::names(loggerNames);
         for (const auto& loggerName : loggerNames)
         {
            Poco::Logger::get(loggerName).setChannel(m_formattingFileChannel);
            Poco::Logger::get(loggerName).setLevel(logLevel);
         }
         loggerNames.clear();

         //configure root logger
         Poco::Logger::root().setChannel(m_formattingFileChannel);
         Poco::Logger::root().setLevel(logLevel);
      }
   }
} // namespace shared::process


