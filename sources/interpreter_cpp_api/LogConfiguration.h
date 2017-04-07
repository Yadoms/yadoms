#pragma once
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>

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
      Poco::AutoPtr<Poco::PatternFormatter> m_patternFormatter;
      Poco::AutoPtr<Poco::FormattingChannel> m_formattingFileChannel;
      Poco::AutoPtr<Poco::FileChannel> m_fileChannel;
   };
} // namespace interpreter_cpp_api


