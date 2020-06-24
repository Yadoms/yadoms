#pragma once

namespace shared
{
   namespace http
   {
      //---------------------------------
      ///\brief Utility (static) class for downloading files
      //---------------------------------
      class CFileDownloader
      {
      public:
         CFileDownloader() = delete;
         virtual ~CFileDownloader() = default;

         //---------------------------------
         ///\brief Define a function prototype for updating a download progress
         ///\param [in] message     Message associated to current step
         ///\param [in] progress    Current progress value (0 to 1)
         //---------------------------------
         typedef std::function<void(const std::string& message, float progress)> onProgressFunc;

         //---------------------------------
         ///\brief Download a file
         ///\param [in] url               URL to download : http://yadoms.com/download/file1.zip
         ///\param [in] location          The file download location (file will be created)
         ///\param [in] reporter          A function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return The downloaded location
         //---------------------------------
         static boost::filesystem::path downloadFile(const std::string& url,
                                                     const boost::filesystem::path& location,
                                                     onProgressFunc reporter);

         //---------------------------------
         ///\brief Download a file and check MD5 hash
         ///\param [in] url               The URL to download
         ///\param [in] location          The file download location (file will be created)
         ///\param [in] md5HashExpected   The expected file MD5 hash
         ///\param [in] reporter          A function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return The downloaded location
         ///\throw   boost::system::system_error : if download fails
         ///\throw   shared::exception::CException : if url is not valid
         ///\throw   shared::exception::CException : if md5 hash is not valid
         //---------------------------------
         static boost::filesystem::path downloadFileAndVerify(const std::string& url,
                                                              const boost::filesystem::path& location,
                                                              const std::string& md5HashExpected,
                                                              onProgressFunc reporter);
      };
   } //namespace http
} //namespace shared
