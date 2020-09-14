#include "stdafx.h"
#include "Proxy.h"

namespace shared
{
   namespace http
   {
      std::string CProxy::ProxyHost;
      int CProxy::ProxyPort = kUseProxyDefaultPort;
      std::string CProxy::ProxyUsername;
      std::string CProxy::ProxyPassword;
      std::string CProxy::ProxyBypassRegex;

      void CProxy::setGlobalProxyConfig(const std::string& host,
                                        int port,
                                        const std::string& username,
                                        const std::string& password,
                                        const std::string& bypassRegex)
      {
         ProxyHost = host;
         ProxyPort = port;
         ProxyUsername = username;
         ProxyPassword = password;
         ProxyBypassRegex = bypassRegex;
      }

      bool CProxy::available()
      {
         return !ProxyHost.empty();
      }

      std::string CProxy::getHost()
      {
         return ProxyHost;
      }

      int CProxy::getPort()
      {
         return ProxyPort;
      }

      std::string CProxy::getUsername()
      {
         return ProxyUsername;
      }

      std::string CProxy::getPassword()
      {
         return ProxyPassword;
      }

      std::string CProxy::getBypassRegex()
      {
         return ProxyBypassRegex;
      }
   } //namespace http
} //namespace shared
