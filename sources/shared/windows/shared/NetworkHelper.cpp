#include "stdafx.h"

#include "../../shared/exception/Exception.hpp"
#include "../../shared/Log.h"
#include "NetworkHelper.h"

namespace shared
{

    CNetworkHelper::CNetworkHelper() = default;
    CNetworkHelper::~CNetworkHelper() = default;

    std::vector<boost::asio::ip::address> CNetworkHelper::getLocalIps()
    {
       std::vector<boost::asio::ip::address> result;

       boost::asio::io_context io;
       boost::asio::ip::udp::resolver resolver(io);

       try
       {
          auto results = resolver.resolve(boost::asio::ip::host_name(), "");

          for (const auto& entry : results)
          {
             const auto addr = entry.endpoint().address();
             if (addr.is_v4()) // on ne garde que les adresses IPv4
                result.push_back(addr);
          }
       }
       catch (const std::exception& e)
       {
          YADOMS_LOG(error) << "Erreur lors de la résolution des adresses locales : " << e.what();
          throw shared::exception::CException(std::string("Network resolution failed: ") + e.what());
       }

       return result;
    }

} // namespace shared
