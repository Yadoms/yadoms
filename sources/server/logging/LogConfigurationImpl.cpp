#include "stdafx.h"
#include "LogConfigurationImpl.h"
#include "LogConfigurationException.h"
#include <Poco/Logger.h>

namespace logging
{
   CLogConfigurationImpl::CLogConfigurationImpl()
   :  m_consoleChannel(new Poco::ConsoleChannel),
      m_fileChannel(new Poco::FileChannel()),
      m_patternFormatter(new Poco::PatternFormatter),
      m_splitterChannel(new Poco::SplitterChannel),
      m_patternFormatterPlugin(new Poco::PatternFormatter),
      m_splitterChannelPlugin(new Poco::SplitterChannel)
   {

   }

   CLogConfigurationImpl::~CLogConfigurationImpl()
   {

   }

   void CLogConfigurationImpl::configure(const std::string & logLevel)
   {
      //make pattern
      m_patternFormatter->setProperty("pattern", "%H:%M:%S : %T : [%p] : %t");
      m_patternFormatter->setProperty("times", "local"); //use local datetime

      m_formattingConsoleChannel.assign(new Poco::FormattingChannel(m_patternFormatter, m_consoleChannel));
      
      std::string Logpath("logs");

      if (!boost::filesystem::exists( Logpath ))
         if (!boost::filesystem::create_directories( Logpath ))
            throw CLogConfigurationException( "Cannot create directory " + Logpath );

      m_fileChannel->setProperty("path", Logpath + "/yadoms.log");
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

      m_patternFormatterPlugin->setProperty("pattern", "%t");
      m_formattingFileChannelPlugin.assign(new Poco::FormattingChannel(m_patternFormatterPlugin, m_fileChannel));
      m_formattingConsoleChannelPlugin.assign(new Poco::FormattingChannel(m_patternFormatterPlugin, m_consoleChannel));
      m_splitterChannelPlugin->addChannel(m_formattingConsoleChannelPlugin);
      m_splitterChannelPlugin->addChannel(m_formattingFileChannelPlugin);
      Poco::Logger::create("plugin", m_splitterChannelPlugin); // TODO pourquoi "plugin" dans cet objet commun ?

      //configure root logger
      Poco::Logger::root().setChannel(m_splitterChannel);
      Poco::Logger::root().setLevel(logLevel);
   }

} // namespace logging