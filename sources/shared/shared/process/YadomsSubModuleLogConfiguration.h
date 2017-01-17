#pragma once
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include "CoutCerrConsoleLogChannel.h"

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \class The interpreter log configuration
      //--------------------------------------------------------------
      class CYadomsSubModuleLogConfiguration
      {
      public:
         //--------------------------------------------------------------
         /// \brief Constructor
         //--------------------------------------------------------------
         CYadomsSubModuleLogConfiguration();

         //--------------------------------------------------------------
         /// \brief Destructor
         //--------------------------------------------------------------
         virtual ~CYadomsSubModuleLogConfiguration();

         //--------------------------------------------------------------
         /// \brief Configure the interpreter logger, with a log level and a file path
         /// \param [in] logLevel      The requested log level
         /// \param [in] logfilepath   The log file path. (the file will be managed by the Poco::Logger using file rotation per day)
         //--------------------------------------------------------------
         void configure(const std::string& logLevel,
                        boost::filesystem::path& logfilepath);
      private:
         Poco::AutoPtr<Poco::PatternFormatter> m_consolePatternFormatter;
         Poco::AutoPtr<Poco::FormattingChannel> m_formattingConsoleChannel;
         Poco::AutoPtr<CCoutCerrConsoleLogChannel> m_consoleChannel;

         Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
         Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
         Poco::AutoPtr<Poco::FileChannel> m_fileChannel;

         Poco::AutoPtr<Poco::SplitterChannel> m_splitterChannel;
      };
   }
} // namespace shared::process


