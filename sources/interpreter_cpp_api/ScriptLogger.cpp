#include "stdafx.h"
#include "ScriptLogger.h"
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <shared/Log.h>
#include <shared/StringExtension.h>


namespace interpreter_cpp_api
{
   CScriptLogger::CScriptLogger(int scriptInstanceId,
                                const boost::filesystem::path& scriptLogPath)
      : m_logger(Poco::Logger::get("Rule." + std::to_string(scriptInstanceId))),
        m_scriptLogPath(scriptLogPath)
   {
      Poco::AutoPtr<Poco::PatternFormatter> patternFormatter(new Poco::PatternFormatter);
      Poco::AutoPtr<Poco::FormattingChannel> formattingFileChannel;
      Poco::AutoPtr<Poco::FileChannel> fileChannel(new Poco::FileChannel());

      patternFormatter->setProperty("pattern", "%w, %H:%M:%S : [%p] : %t");
      patternFormatter->setProperty("times", "local"); //use local datetime

      if (!boost::filesystem::exists(scriptLogPath.parent_path().string()))
         if (!boost::filesystem::create_directories(scriptLogPath.parent_path().string()))
            throw std::runtime_error((boost::format("Cannot create directory %1%") % scriptLogPath.parent_path()).str());

      fileChannel->setProperty("times", "local"); //use local datetime for rotation strategy
      fileChannel->setProperty("path", scriptLogPath.string());
      fileChannel->setProperty("rotation", "weekly");
      fileChannel->setProperty("archive", "timestamp");
      fileChannel->setProperty("compress", "true");
      fileChannel->setProperty("purgeCount", "7");
      formattingFileChannel.assign(new Poco::FormattingChannel(patternFormatter,
                                                               fileChannel));

      m_logger.setChannel(formattingFileChannel);
      m_logger.setLevel("information");
   }

   CScriptLogger::~CScriptLogger()
   {
      m_logger.destroy(m_logger.name());
   }

   void CScriptLogger::init()
   {
   }

   void CScriptLogger::information(const std::string& line)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_logMutex);
      m_logger.information(shared::CStringExtension::removeEol(line));
   }

   void CScriptLogger::error(const std::string& line)
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_logMutex);
      m_logger.error(shared::CStringExtension::removeEol(line));
   }

   void CScriptLogger::purgeLogFile()
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_logMutex);

      auto channel = m_logger.getChannel();

      channel->close();

      purgeLogFile(m_scriptLogPath.parent_path());

      channel->open();
   }

   void CScriptLogger::purgeLogFile(const boost::filesystem::path& scriptLogDirectory)
   {
      // Remove current and rotated files
      for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(scriptLogDirectory), {}))
      {
         try
         {
            boost::filesystem::remove_all(entry);
         }
         catch (std::exception& exception)
         {
            YADOMS_LOG(warning) << "CScriptLogger::purgeLogFile : Unable to delete " << entry << "file, " << exception.what();
         }
      }
   }
} // namespace interpreter_cpp_api	


