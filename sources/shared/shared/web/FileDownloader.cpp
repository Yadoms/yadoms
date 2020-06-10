#include "stdafx.h"
#include "FileDownloader.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/encryption/Md5.h>
#include <Poco/URI.h>
#include <Poco/URIStreamOpener.h>
#include <Poco/File.h>
#include <Poco/StreamCopier.h>
#include "exception/InvalidHash.hpp"
#include "exception/DownloadFailed.hpp"
#include <fstream>
#include <shared/http/HttpMethods.h>

using boost::asio::ip::tcp;

namespace shared
{
   namespace web
   {
      void CFileDownloader::reportProgressToLog(const std::string& info,
                                                float progression)
      {
         YADOMS_LOG(information) << "Downloading " << info << " : " << boost::format("%11.0f") % progression << " %";
      }

      int CFileDownloader::downloadFile(const std::string& url,
                                        std::ostream& output,
                                        ProgressFunc reporter)
      {
         //TODO gérer la progression
         try
         {
            const std::unique_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(Poco::URI(url)));
            return Poco::StreamCopier::copyStream(*pStr, output);
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

      Poco::Path CFileDownloader::downloadFile(const Poco::URI& toDownload,
                                               const Poco::Path& location,
                                               ProgressFunc reporter)
      {
         //create stream
         std::ofstream packageLocalFileStream(location.toString().c_str(), std::ios::binary);

         //download file
         const auto fileSize = downloadFile(toDownload.toString(),
                                            packageLocalFileStream,
                                            reporter);
         packageLocalFileStream.close();
         //check file is downloaded
         const Poco::File packageFile(location);
         if (!packageFile.exists() || fileSize == 0)
         {
            if (fileSize == 0)
               throw exception::CDownloadFailed(toDownload.toString(), "File size is 0");

            throw exception::CDownloadFailed(toDownload.toString(), "Local downloaded file do not exists");
         }

         return location;
      }

      Poco::Path CFileDownloader::downloadFileAndVerify(const Poco::URI& toDownload,
                                                        const Poco::Path& location,
                                                        const std::string& md5HashExpected,
                                                        ProgressFunc reporter)
      {
         const auto result = downloadFile(toDownload,
                                          location,
                                          reporter);

         //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
         const auto md5HashCalculated = encryption::CMd5::digestFile(location.toString());
         if (!boost::iequals(md5HashCalculated, md5HashExpected))
         {
            //fail to verify checksum
            //remove file
            Poco::File packageFile(location);
            packageFile.remove();
            throw exception::CInvalidHash(location, md5HashExpected, md5HashCalculated);
         }

         return result;
      }
   } //namespace web
} //namespace shared
