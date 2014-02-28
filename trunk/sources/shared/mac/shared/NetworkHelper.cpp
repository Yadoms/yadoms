#include "stdafx.h"

#include "../../shared/exception/Exception.hpp"
#include "../../shared/Log.h"
#include "NetworkHelper.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

namespace shared
{

CNetworkHelper::CNetworkHelper()
{
}

CNetworkHelper::~CNetworkHelper()
{
}

std::vector<boost::asio::ip::address> CNetworkHelper::getLocalIps()
{
   std::vector<boost::asio::ip::address> result;
   struct ifaddrs *myaddrs, *ifa;
   void *in_addr;

   if(getifaddrs(&myaddrs) != 0)
       throw shared::exception::CException("Unable to list local IPs");

   for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
   {
      if (ifa->ifa_addr == NULL)
          continue;
      if (!(ifa->ifa_flags & IFF_UP))
          continue;

      switch (ifa->ifa_addr->sa_family)
      {
          case AF_INET:
          {
              struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
              in_addr = &s4->sin_addr;
               
              //we convert in_addr to string
              char str[INET_ADDRSTRLEN];

              if (!inet_ntop(AF_INET, in_addr, str, INET_ADDRSTRLEN))
                 throw shared::exception::CException("Unable to get IP object from in_addr object");
                  
              //we convert string to boost::asio::ip::address
              boost::system::error_code ec;
              boost::asio::ip::address addr = boost::asio::ip::address::from_string(str, ec);
              if (ec)
                 throw shared::exception::CException("Unable to get IP object from ip string : " + std::string(str) + ": " + ec.message());
               
              result.push_back(addr);
              break;
          }

          case AF_INET6:
          {
              //we don't manage ipv6
              break;
          }

           default:
             continue;
       }
   }

   freeifaddrs(myaddrs);
   return result;
}

} // namespace shared