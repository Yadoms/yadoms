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
            if (const auto& ptr = c.lock())
               ptr->close();
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
         {
            m_connections.emplace(std::static_pointer_cast<oatpp::network::tcp::Connection>(connection));
            // The following code allows to limit the memory consumption in case of loss of shared_ptr following a close from a client.
            auto it = m_connections.begin();
            while (it != m_connections.end())
            {
               // We copy the current iterator then we increment it.
               const auto current = it++;
               if ((*current).lock() == nullptr)
               {
                  // we don't invalidate the iterator, because it already points to the next element.
                  m_connections.erase(current);
               }
            }
         }
         return connection;
      }
   } //namespace oatppServer
} //namespace web
