#include "stdafx.h"
#include "FileDownloader.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidHash.hpp>
#include <shared/exception/DownloadFailed.hpp>
#include <shared/encryption/Md5.h>
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <shared/http/HttpMethods.h>

using boost::asio::ip::tcp;

namespace shared
{
   namespace http
   {
      void CFileDownloader::reportProgressToLog(const std::string& info,
                                                float progression)
      {
         YADOMS_LOG(information) << "Downloading " << info << " : " << boost::format("%11.0f") % progression << " %";
      }

      long long CFileDownloader::downloadFile(const std::string& url,
                                              std::ostream& output,
                                              ProgressFunc reporter)
      {
         //TODO gérer la progression
         try
         {
            const std::unique_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(Poco::URI(url)));
            return Poco::StreamCopier::copyStream(*pStr, output); //TODO warning MSVC à résoudre
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Fail to download file : " << e.what();
            throw;
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Fail to download file, unknown error";
            throw;
         }
      }

      boost::filesystem::path CFileDownloader::downloadFile(const std::string& url,
                                                            const boost::filesystem::path& location,
                                                            ProgressFunc reporter)
      {
         //create stream
         std::ofstream packageLocalFileStream(location.string(), std::ios::binary);

         //download file
         const auto fileSize = downloadFile(url,
                                            packageLocalFileStream,
                                            reporter);
         packageLocalFileStream.close();

         //check file is downloaded
         if (!exists(location) || fileSize == 0)
         {
            if (fileSize == 0)
               throw exception::CDownloadFailed(url, "File size is 0");

            throw exception::CDownloadFailed(url, "Local downloaded file do not exists");
         }

         return location;
      }

      boost::filesystem::path CFileDownloader::downloadFileAndVerify(const std::string& url,
                                                                     const boost::filesystem::path& location,
                                                                     const std::string& md5HashExpected,
                                                                     ProgressFunc reporter)
      {
         const auto result = downloadFile(url,
                                          location,
                                          reporter);

         //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
         const auto md5HashCalculated = encryption::CMd5::digestFile(location.string());
         if (!boost::iequals(md5HashCalculated, md5HashExpected))
         {
            //fail to verify checksum
            //remove file
            boost::filesystem::remove(location);
            throw exception::CInvalidHash(location, md5HashExpected, md5HashCalculated);
         }

         return result;
      }
   } //namespace http
} //namespace shared
