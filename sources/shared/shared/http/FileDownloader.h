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
         //---------------------------------
         typedef boost::function2<void, const std::string&, float> ProgressFunc;

         //---------------------------------
         ///\brief Static method which reports download progress on console
         ///\param [in] info : the filename which is downloading
         ///\param [in] progression : the actual progression
         //---------------------------------
         static void reportProgressToLog(const std::string& info,
                                         float progression);

         //---------------------------------
         ///\brief Download a file
         ///\param [in] url               URL to download : http://yadoms.com/download/file1.zip
         ///\param [in] location          The file download location (file will be created)
         ///\param [in] reporter          A function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return The downloaded location
         //---------------------------------
         static boost::filesystem::path downloadFile(const std::string& url,
                                                     const boost::filesystem::path& location,
                                                     ProgressFunc reporter);

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
                                                              ProgressFunc reporter);
      };
   } //namespace http
} //namespace shared
