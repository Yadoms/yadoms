#pragma once
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

namespace web
{
   namespace oatppServer
   {
      /// @brief Specific connection provider
      /// Needed because of the behaviour described here https://giters.com/oatpp/oatpp/issues/476
      /// When using header "Connection" with "keep-alive" value, the connection is never closed
      /// and closing the server never close connections and result on a crash (at least under Windows).
      /// So use a specific connection provider to close connections (release sockets)
      class CConnectionProvider final : public oatpp::network::tcp::server::ConnectionProvider
      {
      public:
         CConnectionProvider(const oatpp::network::Address& address,
                             bool useExtendedConnections = false);
         ~CConnectionProvider() override;

         static std::shared_ptr<CConnectionProvider> createShared(const oatpp::network::Address& address,
                                                                  bool useExtendedConnections = false);

         std::shared_ptr<oatpp::data::stream::IOStream> get() override;

      private:
         using WeakConnection = std::weak_ptr<oatpp::network::tcp::Connection>;

         struct LessCompare
         {
            bool operator()(const WeakConnection& lhs,
                            const WeakConnection& rhs) const
            {
               const auto lPtr = lhs.lock();
               const auto rPtr = rhs.lock();
               if (!rPtr) return false; // nothing after expired pointer 
               if (!lPtr) return true; // every not expired after expired pointer
               return lPtr < rPtr;
            }
         };

         std::set<WeakConnection, LessCompare> m_connections;
      };
   } //namespace oatppServer
} //namespace web
