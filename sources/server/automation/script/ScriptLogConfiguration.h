#pragma once

#include <Poco/ConsoleChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Logger.h>

namespace automation
{
   namespace script
   {
      //--------------------------------------------------------------
      /// \class The script log configuration
      //--------------------------------------------------------------
      class CScriptLogConfiguration//TODO utile ?
      {
      public:
         //--------------------------------------------------------------
         /// \brief Constructor
         //--------------------------------------------------------------
         CScriptLogConfiguration();

         //--------------------------------------------------------------
         /// \brief Destructor
         //--------------------------------------------------------------
         virtual ~CScriptLogConfiguration();

         //--------------------------------------------------------------
         /// \brief Configure the script logger, with a log level and a file path
         /// \param [in] logger        The logger to configure
         /// \param [in] logLevel      The requested log level
         /// \param [in] logfilepath   The log file path. (the file will be managed by the Poco::Logger using file rotation per day)
         //--------------------------------------------------------------
         void configure(Poco::Logger& logger,
                        const std::string& logLevel,
                        const boost::filesystem::path& logfilepath);
      private:
         //--------------------------------------------------------------
         /// \brief The console channel
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::ConsoleChannel> m_consoleChannel;
         //--------------------------------------------------------------
         /// \brief The file channel (configured with logfilepath parameter of configure method)
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::FileChannel> m_fileChannel;
         //--------------------------------------------------------------
         /// \brief The output pattern formatter
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
         //--------------------------------------------------------------
         /// \brief The output pattern formatter for console
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::FormattingChannel> m_formattingConsoleChannel;
         //--------------------------------------------------------------
         /// \brief The output pattern formatter for file
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
         //--------------------------------------------------------------
         /// \brief The splitter channel which allow to use more than one single channel
         //--------------------------------------------------------------
         Poco::AutoPtr<Poco::SplitterChannel> m_splitterChannel;
      };
   }
} // namespace automation::script


