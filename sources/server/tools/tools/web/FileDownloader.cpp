#include "stdafx.h"
#include "FileDownloader.h"
#include <boost/regex.hpp>
#include <shared/Log.h>
#include <shared/exception/Exception.hpp>
#include "Uri.h"

using boost::asio::ip::tcp;

namespace tools { namespace web { 

   void CFileDownloader::reportProgressToLog(const std::string & info, float progression)
   {
      YADOMS_LOG(information) << "Downloading " << info << " : " << boost::format("%11.0f") %  progression << " %";
   }

   int CFileDownloader::downloadFile(const CUri & uri, std::ostream & output, ProgressFunc reporter)
   {
      try
      {
         if(uri.isValid())
         {
            boost::asio::io_service io_service;

            // Get a list of endpoints corresponding to the server name.
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(uri.getUriDomain(), uri.getUriSchema());
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            tcp::socket socket(io_service);
            boost::asio::connect(socket, endpoint_iterator);

            // Form the request. We specify the "Connection: close" header so that the
            // server will close the socket after transmitting the response. This will
            // allow us to treat all data up until the EOF as the content.
            boost::asio::streambuf request;
            std::ostream request_stream(&request);
            request_stream << "GET " << uri.getUriPath() << uri.getUriFile() << " HTTP/1.0\r\n";
            request_stream << "Host: " << uri.getUriDomain() << "\r\n";
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
               std::cout << "Invalid response\n";
               return 1;
            }
            if (status_code != 200)
            {
               std::cout << "Response returned with status code " << status_code << "\n";
               return 1;
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
            if (response.size() > 0 && contentLength >0)
            {
                  currentReadSize = response.size();
                  output << &response;
                  currentprogress = (currentReadSize*100.0f/(float)contentLength);
                  reporter(uri.getUriFile(), currentprogress);
            }
               
            

            
            // Read until EOF, writing data to output as we go.
            boost::system::error_code error;
            while (boost::asio::read(socket, response, boost::asio::transfer_at_least(50000), error))
            {
               //if an error occurs during download, then stop
               if (error != boost::asio::error::eof && error.value() != 0)
               {
                  YADOMS_LOG(error) << error.message();
                  YADOMS_LOG(error) << error.category().name();
                  YADOMS_LOG(error) << error.value();

                  throw boost::system::system_error(error);
               }

               //get the size of the read part
               currentReadSize += response.size();

               //write it to stream
               output << &response;

               //update progress if needed (minimum step of 1% to avoid too much calls)
               //then flush by 1% minimum steps
               float progress = (currentReadSize*100.0f/(float)contentLength);
               if(abs(currentprogress - progress) >= 1.0)
               {
                  currentprogress = progress;
                  reporter(uri.getUriFile(), currentprogress);
                  output.flush();
               }
            }

            //check that it ends prematurely
            if (error != boost::asio::error::eof)
               throw boost::system::system_error(error);

            //all is good, return read size
            return currentReadSize;
         }
         else
         {
            throw shared::exception::CException("The uri is invalid");
         }
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Fail to download file : " << e.what() << "\n";
         throw;
      }      
   }


   int CFileDownloader::downloadFile(const std::string & url, std::ostream & output, ProgressFunc reporter)
   {
      return downloadFile(CUri(url), output, reporter);
   }

} //namespace web
} //namespace tools