#pragma once

#include "Uri.h"

namespace tools { namespace web { 

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
      static void reportProgressToLog(const std::string & info, float progression);

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
      static int downloadFile(const std::string & url, std::ostream & output, ProgressFunc reporter);

      //---------------------------------
      ///\brief Download a file
      ///\param [in] uri : a valid URI
      ///\param [in] output : a stream which will receive download bytes
      ///\param [in] reporter : a function pointer for reporting progress (can be used with CFileDownloader::reportProgressToLog)
      ///\return the number of bytes read
      ///\throw   boost::system::system_error : if download fails
      ///\throw   shared::exception::CException : if url is not valid
      //---------------------------------
      static int downloadFile(const CUri & uri, std::ostream & output, ProgressFunc reporter);
   };

} //namespace web
} //namespace tools
