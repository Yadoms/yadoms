#pragma once
#include <string>

namespace web
{
   namespace rest
   {
      namespace service
      {
         /// @brief Server-Send-Event connection handler interface
         /// @details Implements this interface to send events to the server asynchronously
         class ISseConnectionHandler
         {
         public:
            virtual ~ISseConnectionHandler() = default;

            ISseConnectionHandler() = default;
            ISseConnectionHandler(const ISseConnectionHandler&) = delete;
            ISseConnectionHandler(ISseConnectionHandler&&) = delete;
            ISseConnectionHandler& operator=(const ISseConnectionHandler&) = delete;
            ISseConnectionHandler& operator=(ISseConnectionHandler&&) = delete;

            /// @brief Blocking function waiting for event to send to server
            /// @param timeout Timeout for wait function : !! MUST BE USED !! (see note below)
            /// @return Pair of strings (event name, event data)
            /// @note Handle should not block more than HandleTimeout to let chance to free resources if client has closed the connection
            virtual std::pair<std::string, std::string> handle(const boost::posix_time::time_duration& timeout) const = 0;
         };
      }
   } //namespace oatppServer
} //namespace web
