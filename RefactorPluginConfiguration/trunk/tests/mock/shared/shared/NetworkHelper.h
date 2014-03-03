#pragma once

namespace shared
{
   class CNetworkHelper
   {
   public:
      virtual ~CNetworkHelper() {}

      // Mock : returns 2 IPs
      static std::vector<boost::asio::ip::address> getLocalIps()
      {
         std::vector<boost::asio::ip::address> result = std::vector<boost::asio::ip::address>();
         result.push_back(boost::asio::ip::address::from_string("192.168.0.1"));
         result.push_back(boost::asio::ip::address::from_string("192.168.0.2"));
         return result;
      }

   private:
      CNetworkHelper() {}
   };

} // namespace shared