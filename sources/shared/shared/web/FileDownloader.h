#pragma once

#include <shared/DataContainer.h>
#include <Poco/URI.h>
#include <Poco/Path.h>

namespace shared
{
   namespace web
   {
      //---------------------------------
      ///\brief Utility (static) class for downloading files
      //---------------------------------
      class CFileDownloader
      {
      private:
         //---------------------------------
         ///\brief Private constructor. this class should be yused with static methods only
         //---------------------------------
         CFileDownloader()
         {
         }

         //---------------------------------
         ///\brief Private destructor. this class should be yused with static methods only
         //---------------------------------
         virtual ~CFileDownloader()
         {
         }

      public:
         //---------------------------------
         ///\brief Define a function prototype for updating a download progress
         //---------------------------------
         typedef boost::function2<void, const std::string &, float> ProgressFunc;

         //---------------------------------
         ///\brief Static method which reports download progress on console
         ///\param [in] info : the filename which is downloading
         ///\param [in] progression : the actual progression
         //---------------------------------
         static void reportProgressToLog(const std::string& info, float progression);

         //---------------------------------
         ///\brief Download a file
         ///\param [in] url : the url to download : http://yadoms.com/download/file1.zip
         ///\param [in] output : a stream which will receive download bytes
         ///\param [in] reporter : a function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return the number of bytes read
         ///\throw   boost::system::system_error : if download fails
         ///\throw   shared::exception::CException : if url is not valid
         ///\example: tools::web::CFileDownloader::downloadFile("http://yadoms.com/download/file1.zip", outfile, boost::bind(&tools::web::CFileDownloader::reportProgressToLog, _1, _2));
         //---------------------------------
         static int downloadFile(const std::string& url, std::ostream& output, ProgressFunc reporter);

         //---------------------------------
         ///\brief Download a file
         ///\param [in] uri : a valid URI
         ///\param [in] output : a stream which will receive download bytes
         ///\param [in] reporter : a function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return the number of bytes read
         ///\throw   boost::system::system_error : if download fails
         ///\throw   shared::exception::CException : if url is not valid
         //---------------------------------
         static int downloadFile(const Poco::URI& uri, std::ostream& output, ProgressFunc reporter);


         //---------------------------------
         ///\brief Download a JSON file (in memory)
         ///\param [in] jsonPath : the JSON file to read
         ///\param [in] reporter : a function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return A DataContainer which handles the JSON content
         //---------------------------------
         static shared::CDataContainer downloadInMemoryJsonFile(const Poco::URI& jsonPath, ProgressFunc reporter);

         //---------------------------------
         ///\brief Download a file and check MD5 hash
         ///\param [in] toDownload        The URI to download
         ///\param [in] location          The file download location (file will be created)
         ///\param [in] md5HashExpected   The expected file MD5 hash
         ///\param [in] reporter          A function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return The downloaded location
         ///\throw   boost::system::system_error : if download fails
         ///\throw   shared::exception::CException : if url is not valid
         ///\throw   shared::exception::CException : if md5 hash is not valid
         //---------------------------------
         static Poco::Path downloadFileAndVerify(const Poco::URI& toDownload, const Poco::Path& location, const std::string& md5HashExpected, ProgressFunc reporter);

         //---------------------------------
         ///\brief Download a file
         ///\param [in] toDownload        The URI to download
         ///\param [in] location          The file download location (file will be created)
         ///\param [in] reporter          A function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
         ///\return The downloaded location
         ///\throw   boost::system::system_error : if download fails
         ///\throw   shared::exception::CException : if url is not valid
         //---------------------------------
         static Poco::Path downloadFile(const Poco::URI& toDownload, const Poco::Path& location, ProgressFunc reporter);
      };
   } //namespace web
} //namespace shared
