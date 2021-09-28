#include "stdafx.h"
#include "ConnectionProvider.h"

namespace web
{
   namespace oatppServer
   {
      CConnectionProvider::CConnectionProvider(const oatpp::network::Address& address,
                                               bool useExtendedConnections)
         : ConnectionProvider(address,
                              useExtendedConnections)
      {
      }

      CConnectionProvider::~CConnectionProvider()
      {
         for (const auto& c : m_connections)
            c->close();
      }

      std::shared_ptr<CConnectionProvider> CConnectionProvider::createShared(const oatpp::network::Address& address,
                                                                             bool useExtendedConnections)
      {
         return std::make_shared<CConnectionProvider>(address,
                                                      useExtendedConnections);
      }

      std::shared_ptr<oatpp::data::stream::IOStream> CConnectionProvider::get()
      {
         auto connection = ConnectionProvider::get();
         if (connection)
            m_connections.emplace(std::static_pointer_cast<oatpp::network::tcp::Connection>(connection));
         return connection;
      }
   } //namespace oatppServer
} //namespace web
