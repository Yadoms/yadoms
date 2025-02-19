#include "stdafx.h"
#include "FileDownloader.h"
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include <shared/exception/InvalidHash.hpp>
#include <shared/exception/DownloadFailed.hpp>
#include <shared/encryption/Md5.h>
#include <Poco/StreamCopier.h>
#include <fstream>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Infos.hpp>
#include <utility>
#include "Proxy.h"
#include "curlppHelpers.h"
#include "shared/exception/HttpException.hpp"

namespace shared
{
   namespace http
   {
      boost::filesystem::path CFileDownloader::downloadFile(const std::string& url,
                                                            const boost::filesystem::path& location,
                                                            onProgressFunc reporter)
      {
         curlpp::Easy request;

         // URL
         request.setOpt(new curlpp::options::Url(url));

         // Headers
         CCurlppHelpers::setHeaders(request, std::map<std::string, std::string>
                                    {
                                       {"User-Agent", "yadoms"},
                                       {"Connection", "close"}
                                    });

         // Proxy
         CCurlppHelpers::setProxy(request,
                                  url);

         // Follow redirections
         request.setOpt(new curlpp::options::FollowLocation(true));
         request.setOpt(new curlpp::options::MaxRedirs(3));

         // HTTPS support : skip peer and host verification
         static const std::string HttpsHeader("https://");
         if (std::search(url.begin(), url.end(),
                         HttpsHeader.begin(), HttpsHeader.end(),
                         [](const char ch1, const char ch2)
                         {
                            return std::tolower(ch1) == std::tolower(ch2);
                         })
            != url.end())
         {
            request.setOpt(new curlpp::options::SslVerifyPeer(false));
            request.setOpt(new curlpp::options::SslVerifyHost(false));
         }

         // Progress
         request.setOpt(new curlpp::options::NoProgress(false));
         request.setOpt(new curlpp::options::ProgressFunction(
            [&location, &reporter](const double totalDownloaded, const double currentlyDownloaded, double, double)-> int
            {
               if (totalDownloaded != 0.0)
                  reporter(location.string(), static_cast<float>(currentlyDownloaded * 100.0f / totalDownloaded));
               return CURL_PROGRESSFUNC_CONTINUE;
            }));

         // Downloaded file
         std::ofstream localFileStream(location.string(), std::ios::binary);
         request.setOpt(curlpp::options::WriteFunction(
            [&localFileStream](char* ptr, size_t size, size_t nbItems)
            {
               const auto incomingSize = size * nbItems;
               localFileStream.write(ptr, incomingSize);
               return incomingSize;
            }));

         try
         {
            request.perform();
         }
         catch (const curlpp::LibcurlRuntimeError& error)
         {
            const auto message = (boost::format("Fail to download %1% : %2%, code %3%")
               % url % error.what() % error.whatCode()).str();
            YADOMS_LOG(warning) << message;
            if (ECodes::isDefined(curlpp::infos::ResponseCode::get(request)))
               throw exception::CHttpException(message, ECodes(curlpp::infos::ResponseCode::get(request)));
            throw std::runtime_error(message);
         }

         CCurlppHelpers::checkResult(request);

         localFileStream.close();

         //check file is downloaded
         if (!exists(location))
            throw exception::CDownloadFailed(
               (boost::format("Fail to download %1%, file doesn't exist on drive") % url).str());

         if (file_size(location) == 0)
            throw exception::CDownloadFailed((boost::format("Fail to download %1%, file size is 0") % url).str());

         return location;
      }

      boost::filesystem::path CFileDownloader::downloadFileAndVerify(const std::string& url,
                                                                     const boost::filesystem::path& location,
                                                                     const std::string& md5HashExpected,
                                                                     onProgressFunc reporter)
      {
         const auto result = downloadFile(url,
                                          location,
                                          std::move(reporter));

         //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
         const auto md5HashCalculated = encryption::CMd5::digestFile(result.string());
         if (!boost::iequals(md5HashCalculated, md5HashExpected))
         {
            //fail to verify checksum
            //remove file
            boost::filesystem::remove(result);
            throw exception::CInvalidHash(result, md5HashExpected, md5HashCalculated);
         }

         return result;
      }
   } //namespace http
} //namespace shared
