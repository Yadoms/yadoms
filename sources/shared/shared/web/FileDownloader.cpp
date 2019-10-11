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
      void CFileDownloader::reportProgressToLog(const std::string& info, float progression)
      {
         YADOMS_LOG(information) << "Downloading " << info << " : " << boost::format("%11.0f") % progression << " %";
      }

      int CFileDownloader::downloadFile(const Poco::URI& uri, std::ostream& output, ProgressFunc reporter)
      {
         //TODO gérer la progressions
         try
         {
            std::unique_ptr<std::istream> pStr(Poco::URIStreamOpener::defaultOpener().open(uri));
            Poco::StreamCopier::copyStream(*pStr, output);
            return pStr->gcount();//TODO hmmm...
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

         return 0; //TODO ménage


         try
         {
            boost::asio::io_service io_service;

            // Get a list of endpoints corresponding to the server name.
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(uri.getHost(), uri.getScheme());
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            // Form the request. We specify the "Connection: close" header so that the
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "GET " << uri.getPathAndQuery() << " HTTP/1.0\r\n";
            request_stream << "Host: " << uri.getHost() << ":" << uri.getPort() << "\r\n";
            request_stream << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
            request_stream << "User-Agent: yadoms\r\n";
            request_stream << "Connection: close\r\n\r\n";

            // Send the request.
            boost::asio::write(socket, request);

            // Read the response status line. The response streambuf will automatically
            // grow to accommodate the entire line. The growth may be limited by passing
            // a maximum size to the streambuf constructor.
            boost::asio::streambuf response;
            boost::asio::read_until(socket, response, "\r\n");

            // Check that response is OK.
            std::istream response_stream(&response);
            std::string http_version;
            response_stream >> http_version;
            unsigned int status_code;
            response_stream >> status_code;
            std::string status_message;
            std::getline(response_stream, status_message);
            if (!response_stream || http_version.substr(0, 5) != "HTTP/")
            {
               throw exception::CDownloadFailed(uri.toString(), "Invalid response : " + http_version);
            }
            if (status_code != 200)
            {
               throw exception::CDownloadFailed(uri.toString(),
                                                "Response returned with status code : " + boost::lexical_cast<std::
                                                   string>(status_code));
            }

            // Read the response headers, which are terminated by a blank line.
            boost::asio::read_until(socket, response, "\r\n\r\n");

            // Process the response headers.
            int contentLength = -1;
            std::string header;
            std::string contentLengthHeader = "Content-Length:";
            boost::to_lower(contentLengthHeader);

            while (std::getline(response_stream, header) && header != "\r")
            {
               //ensure case to lower
               boost::to_lower(header);
               if (boost::starts_with(header, contentLengthHeader))
               {
                  std::string sContentLength = header.substr(contentLengthHeader.size());
                  boost::trim(sContentLength);
                  contentLength = boost::lexical_cast<int>(sContentLength);
               }
            }

            //Process content data

            int currentReadSize = 0;
            float currentprogress = 0;

            // Write whatever content we already have to output.
            if (response.size() > 0 && contentLength > 0)
            {
               currentReadSize = response.size();
               output << &response;
               currentprogress = (currentReadSize * 100.0f / static_cast<float>(contentLength));
               reporter(uri.getPath(), currentprogress);
            }


            // Read until EOF, writing data to output as we go.
            boost::system::error_code error;
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(50000), error))
            {
               //if an error occurs during download, then stop
               if (error != boost::asio::error::eof && error.value() != 0)
               {
                  throw exception::CDownloadFailed(uri, error);
               }

               //get the size of the read part
               currentReadSize += response.size();

               //write it to stream
               output << &response;

               if (contentLength > 0)
               {
                  //update progress if needed (minimum step of 1% to avoid too much calls)
                  //then flush by 1% minimum steps
                  float progress = (currentReadSize * 100.0f / static_cast<float>(contentLength));
                  if (fabs(currentprogress - progress) >= 1.0)
                  {
                     currentprogress = progress;
                     reporter(uri.getPath(), currentprogress);
                     output.flush();
                  }
               }
               else
               {
                  //if content length is not defined (-1), just flush each time
                  output.flush();
               }
            }

            //check that it ends prematurely
            if (error != boost::asio::error::eof)
               throw exception::CDownloadFailed(uri, error);

            //all is good, return read size
            return currentReadSize;
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Fail to download file : " << e.what() << "\n";
            throw;
         }
      }

      //TODO virer 
      //int CFileDownloader::downloadFile(const Poco::URI& uri, std::ostream& output, ProgressFunc reporter)
      //{
      // try
      // {
      //  boost::asio::io_service io_service;

      //  // Get a list of endpoints corresponding to the server name.
      //  tcp::resolver resolver(io_service);
      //  tcp::resolver::query query(uri.getHost(), uri.getScheme());
      //  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

      //  // Try each endpoint until we successfully establish a connection.
      //  tcp::socket socket(io_service);
      //  boost::asio::connect(socket, endpoint_iterator);

      //  // Form the request. We specify the "Connection: close" header so that the
      //  // server will close the socket after transmitting the response. This will
      //  // allow us to treat all data up until the EOF as the content.
      //  boost::asio::streambuf request;
      //  std::ostream request_stream(&request);
      //  request_stream << "GET " << uri.getPathAndQuery() << " HTTP/1.0\r\n";
      //  request_stream << "Host: " << uri.getHost() << ":" << uri.getPort() << "\r\n";
      //  request_stream << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
      //  request_stream << "User-Agent: yadoms\r\n";
      //  request_stream << "Connection: close\r\n\r\n";

      //  // Send the request.
      //  boost::asio::write(socket, request);

      //  // Read the response status line. The response streambuf will automatically
      //  // grow to accommodate the entire line. The growth may be limited by passing
      //  // a maximum size to the streambuf constructor.
      //  boost::asio::streambuf response;
      //  boost::asio::read_until(socket, response, "\r\n");

      //  // Check that response is OK.
      //  std::istream response_stream(&response);
      //  std::string http_version;
      //  response_stream >> http_version;
      //  unsigned int status_code;
      //  response_stream >> status_code;
      //  std::string status_message;
      //  std::getline(response_stream, status_message);
      //  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
      //  {
      //	  throw exception::CDownloadFailed(uri.toString(), "Invalid response : " + http_version);
      //  }
      //  if (status_code != 200)
      //  {
      //	  throw exception::CDownloadFailed(uri.toString(), "Response returned with status code : " + boost::lexical_cast<std::string>(status_code));
      //  }

      //  // Read the response headers, which are terminated by a blank line.
      //  boost::asio::read_until(socket, response, "\r\n\r\n");

      //  // Process the response headers.
      //  int contentLength = -1;
      //  std::string header;
      //  std::string contentLengthHeader = "Content-Length:";
      //  boost::to_lower(contentLengthHeader);

      //  while (std::getline(response_stream, header) && header != "\r")
      //  {
      //	  //ensure case to lower
      //	  boost::to_lower(header);
      //	  if (boost::starts_with(header, contentLengthHeader))
      //	  {
      //		  std::string sContentLength = header.substr(contentLengthHeader.size());
      //		  boost::trim(sContentLength);
      //		  contentLength = boost::lexical_cast<int>(sContentLength);
      //	  }
      //  }

      //  //Process content data

      //  int currentReadSize = 0;
      //  float currentprogress = 0;

      //  // Write whatever content we already have to output.
      //  if (response.size() > 0 && contentLength > 0)
      //  {
      //	  currentReadSize = response.size();
      //	  output << &response;
      //	  currentprogress = (currentReadSize * 100.0f / static_cast<float>(contentLength));
      //	  reporter(uri.getPath(), currentprogress);
      //  }


      //  // Read until EOF, writing data to output as we go.
      //  boost::system::error_code error;
      //  while (boost::asio::read(socket, response, boost::asio::transfer_at_least(50000), error))
      //  {
      //	  //if an error occurs during download, then stop
      //	  if (error != boost::asio::error::eof && error.value() != 0)
      //	  {
      //		  throw exception::CDownloadFailed(uri, error);
      //	  }

      //	  //get the size of the read part
      //	  currentReadSize += response.size();

      //	  //write it to stream
      //	  output << &response;

      //	  if (contentLength > 0)
      //	  {
      //		  //update progress if needed (minimum step of 1% to avoid too much calls)
      //		  //then flush by 1% minimum steps
      //		  float progress = (currentReadSize * 100.0f / static_cast<float>(contentLength));
      //		  if (fabs(currentprogress - progress) >= 1.0)
      //		  {
      //			  currentprogress = progress;
      //			  reporter(uri.getPath(), currentprogress);
      //			  output.flush();
      //		  }
      //	  }
      //	  else
      //	  {
      //		  //if content length is not defined (-1), just flush each time
      //		  output.flush();
      //	  }
      //  }

      //  //check that it ends prematurely
      //  if (error != boost::asio::error::eof)
      //	  throw exception::CDownloadFailed(uri, error);

      //  //all is good, return read size
      //  return currentReadSize;
      // }
      // catch (std::exception& e)
      // {
      //  YADOMS_LOG(error) << "Fail to download file : " << e.what() << "\n";
      //  throw;
      // }
      //}


      int CFileDownloader::downloadFile(const std::string& url, std::ostream& output, ProgressFunc reporter)
      {
         return downloadFile(Poco::URI(url), output, reporter);
      }

      Poco::Path CFileDownloader::downloadFile(const Poco::URI& toDownload, const Poco::Path& location,
                                               ProgressFunc reporter)
      {
         //create stream
         std::ofstream packageLocalFileStream(location.toString().c_str(), std::ios::binary);

         //download file
         int fileSize = downloadFile(toDownload.toString(), packageLocalFileStream, reporter);
         packageLocalFileStream.close();
         //check file is downloaded
         Poco::File packageFile(location);
         if (!packageFile.exists() || fileSize == 0)
         {
            if (fileSize == 0)
               throw exception::CDownloadFailed(toDownload.toString(), "File size is 0");

            throw exception::CDownloadFailed(toDownload.toString(), "Local downloaded file do not exists");
         }

         return location;
      }

      Poco::Path CFileDownloader::downloadFileAndVerify(const Poco::URI& toDownload, const Poco::Path& location,
                                                        const std::string& md5HashExpected, ProgressFunc reporter)
      {
         Poco::Path result = downloadFile(toDownload, location, reporter);

         //we re-read the file and compute the md5 (the md5 can be generated online using ie http://onlinemd5.com/)
         std::string md5HashCalculated = encryption::CMd5::digestFile(location.toString().c_str());
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
