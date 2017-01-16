#pragma once
#include "StandardConsoleChannel.h"
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>

namespace plugin_cpp_api
{
   //--------------------------------------------------------------
   /// \class The plugin log configuration
   //--------------------------------------------------------------
   class CPluginLogConfiguration
   {
   public:
      //--------------------------------------------------------------
      /// \brief Constructor
      //--------------------------------------------------------------
      CPluginLogConfiguration();

      //--------------------------------------------------------------
      /// \brief Destructor
      //--------------------------------------------------------------
      virtual ~CPluginLogConfiguration();

      //--------------------------------------------------------------
      /// \brief Configure the plugin logger, with a log level and a file path
      /// \param [in] logLevel      The requested log level
      /// \param [in] logfilepath   The log file path. (the file will be managed by the Poco::Logger using file rotation per day)
      //--------------------------------------------------------------
      void configure(const std::string& logLevel,
                     boost::filesystem::path& logfilepath);
   private:
      //--------------------------------------------------------------
      /// \brief The console channel
      //--------------------------------------------------------------
      Poco::AutoPtr<StandardConsoleChannel> m_consoleChannel;
      //--------------------------------------------------------------
      /// \brief The file channel (configured with logfilepath parameter of configure method)
      //--------------------------------------------------------------
      Poco::AutoPtr<Poco::FileChannel> m_fileChannel;
      //--------------------------------------------------------------
      /// \brief The output pattern formatter
      //--------------------------------------------------------------
      Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
      //--------------------------------------------------------------
      /// \brief The output pattern formatter for file
      //--------------------------------------------------------------
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
      //--------------------------------------------------------------
      /// \brief The splitter channel which allow to use more than one single channel
      //--------------------------------------------------------------
      Poco::AutoPtr<Poco::SplitterChannel> m_splitterChannel;
   };
} // namespace plugin_cpp_api


