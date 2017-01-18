#include "stdafx.h"
#include "ScriptLogConfiguration.h"

#include <Poco/Logger.h>
#include <shared/exception/Exception.hpp>

namespace automation
{
   namespace script
   {
      CScriptLogConfiguration::CScriptLogConfiguration()
         : m_consoleChannel(new Poco::ConsoleChannel),
           m_fileChannel(new Poco::FileChannel()),
           m_patternFormatter(new Poco::PatternFormatter),
           m_splitterChannel(new Poco::SplitterChannel)
      {
      }

      CScriptLogConfiguration::~CScriptLogConfiguration()
      {
      }

      void CScriptLogConfiguration::configure(Poco::Logger& logger,
                                              const std::string& logLevel,
                                              const boost::filesystem::path& logfilepath)
      {
         //make pattern
         m_patternFormatter->setProperty("pattern", "%H:%M:%S : %s : [%p] : %t");
         m_patternFormatter->setProperty("times", "local"); //use local datetime

         m_formattingConsoleChannel.assign(new Poco::FormattingChannel(m_patternFormatter, m_consoleChannel));

         if (!boost::filesystem::exists(logfilepath.parent_path().string()))
            if (!boost::filesystem::create_directories(logfilepath.parent_path().string()))
               throw shared::exception::CException("Cannot create directory " + logfilepath.parent_path().string());

         m_fileChannel->setProperty("path", logfilepath.string());
         m_fileChannel->setProperty("rotation", "daily");
         m_fileChannel->setProperty("archive", "timestamp");
         m_fileChannel->setProperty("compress", "true");
         m_fileChannel->setProperty("purgeCount", "7");
         m_formattingFileChannel.assign(new Poco::FormattingChannel(m_patternFormatter, m_fileChannel));

         //m_splitterChannel->addChannel(m_formattingConsoleChannel);
         m_splitterChannel->addChannel(m_formattingFileChannel);

         //configure root logger
         logger.setChannel(m_splitterChannel);
         logger.setLevel(logLevel);
      }
   }
} // namespace automation::script


